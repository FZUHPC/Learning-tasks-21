# 											PIVOT

--------------------

### （一）题意简析

 给定n个点，点是dim维的，然后选定k个支撑点，有Cnk个组合，每种组合有一个距离和目标函数的值，我们要给出这些组合从大到小前1000和后1000个的支撑点集合和函数的值。函数的值应该是n个点任意取两个点的所有组合的距离之和，两个点组合的距离为他们到一个支撑点距离之差的绝对值的最大值（k个支撑点中选一个值最大的）

### （二）初始算法简析

穷举所有的支撑点组合，并进行排序，选出前1000大和前1000小的

### （三）优化

#### 1.编译参数优化

-l msmpi -O3 -fopenmp  -march=znver1 -mfma -fomit-frame-pointer -mavx2

#### 2.并行化

采用MPI分布式编程由于不同支撑点组合之间的计算是不互相影响的，由此考虑将支撑组合进行并行划分，考虑选点方式，当一个点必选的时候在他后面的未被必选过的k1个点选k-1个点有Ck1k-1种组合，越往后的点组合的情况越少，考虑到核的负载，在单个线程中中选必选点的时候跳着选。在各个线程计算完成后，发送回0号线程进行归并排序，得出最终答案

核心代码

```c++
if(rank!=0){
        // temp : indexes of pivots with dummy array head
        int* temp = (int*)malloc(sizeof(int) * (k+1));
        temp[0] = -1;
		for(int i=rank-1;i<n-k+1;i+=size-1){
			//printf("%d\n",i);
            temp[0]=i;
			// Main loop. Combine different pivots with recursive function and evaluate them. Complexity : O( n^(k+2) )
	    	Combination(1, k, n, dim, M, coord, temp, maxDistanceSum, maxDisSumPivots, minDistanceSum, minDisSumPivots);
		}
        MPI_Send(maxDistanceSum,M+1, MPI_DOUBLE, 0,0, MPI_COMM_WORLD);
        MPI_Send(maxDisSumPivots,k*(M+1), MPI_INT, 0,1, MPI_COMM_WORLD);
        MPI_Send(minDistanceSum,M+1, MPI_DOUBLE, 0,2, MPI_COMM_WORLD);
        MPI_Send(minDisSumPivots,k*(M+1), MPI_INT, 0,3, MPI_COMM_WORLD);
        
	}
	else{
        double* allMaxDistanceSum = (double*)malloc(sizeof(double)*(size-1)* (M+1));
		int* allMinDisSumPivots = (int*)malloc(sizeof(int) *(size-1)* k * (M+1));
        int* allMaxDisSumPivots = (int*)malloc(sizeof(int) *(size-1)* k * (M+1));
        double* allMinDistanceSum = (double*)malloc(sizeof(double) *(size-1)* (M+1));
        int i;
        for(i=1;i<=size-1; i++) {
			MPI_Recv(&allMaxDistanceSum[(i-1)*(M+1)],sizeof(double) * (M+1), MPI_DOUBLE,i,0, MPI_COMM_WORLD, &status);
            MPI_Recv(&allMaxDisSumPivots[(i-1)*(M+1)*k],sizeof(int) * k * (M+1), MPI_INT,i,1, MPI_COMM_WORLD, &status);
            MPI_Recv(&allMinDistanceSum[(i-1)*(M+1)],sizeof(double)*(M+1), MPI_DOUBLE,i,2, MPI_COMM_WORLD, &status);
            MPI_Recv(&allMinDisSumPivots[(i-1)*(M+1)*k],sizeof(int) * k * (M+1), MPI_INT,i,3, MPI_COMM_WORLD, &status);
		}
        int flag1[size+1],flag2[size+1];
        for(i=0;i<size-1;i++){
            flag1[i]=flag2[i]=0;
        }
        int t;
        for(t=0;t<M;t++){
            int mx=1,mi=1;
            for(i=0;i<size-1;i++){
                if(allMaxDistanceSum[i*(M+1)+flag1[i]]>allMaxDistanceSum[mx*(M+1)+flag1[mx]]) mx=i;
                if(allMinDistanceSum[i*(M+1)+flag2[i]]<allMinDistanceSum[mi*(M+1)+flag2[mi]]) mi=i;
            }
            maxDistanceSum[t]=allMaxDistanceSum[mx*(M+1)+flag1[mx]];
            minDistanceSum[t]=allMinDistanceSum[mi*(M+1)+flag2[mi]];
            int j;
            for(j=0;j<k;j++){
                maxDisSumPivots[t*k+j]=allMaxDisSumPivots[mx*(M+1)*k+flag1[mx]*k+j];
                minDisSumPivots[t*k+j]=allMinDisSumPivots[mi*(M+1)*k+flag2[mi]*k+j];
            }
            flag1[mx]++;
            flag2[mi]++;
        }
```



#### 3.简单算法上的调整

显然性能瓶颈在选择支撑点选择后的距离函数的计算是k*n^2的时间复杂度的

```c++
double chebyshevSum = 0;
    int block=n;
    int l=0,r=0;
   	for(i=0; i<n; i++){
        int j;
        for(j=0; j<n; j++){
            double chebyshev = 0;
            int ki;
            for(ki=0; ki<k; ki++){
                double dis = fabs(rebuiltCoord[i*k + ki] - rebuiltCoord[j*k + ki]);
                chebyshev = dis>chebyshev ? dis : chebyshev;
            }
            chebyshevSum += chebyshev；
        }
    }
```



显然这个计算是对称的，将第二个for循环j初始值改为i+1，可以减少一半的运算使程序加快一倍

#### 4.对算法并行的进一步优化

由于性能瓶颈在于距离函数的运算，所有点组合之间的运算又是相互独立的，于是考虑用gpu并行加速这个过程，由于计算后结果是一个上三角矩阵，每一行的数量递减，于是考虑行之间并行加和，完成行加的列进行列相加，加和的时候进行加锁，感觉理论上来说可以提升n/2倍，由于本机cuda的环境是在windows上安装的vc编译器和nvcc编译器编译文件在链接的时候出现了一些问题，暂时还没有解决QAQ

![](https://flea--market.oss-cn-beijing.aliyuncs.com/bec9941260e0a694925d3d62c808676.png)
