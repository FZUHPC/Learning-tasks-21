#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<sys/time.h>
#include <mpi.h>
#include <bits/stdc++.h>
// Calculate sum of distance while combining different pivots. Complexity : O( n^2 )
double SumDistance(const int k, const int n, const int dim, double* coord, int* pivots){
    double* rebuiltCoord = (double*)malloc(sizeof(double) * n * k);
    int i;
    for(i=0; i<n*k; i++){
        rebuiltCoord[i] = 0;
    }
	//求n个点到支撑点的k个距离 
    // Rebuild coordinates. New coordinate of one point is its distance to each pivot.
    for(i=0; i<n; i++){
        int ki;
        for(ki=0; ki<k; ki++){
            double distance = 0;
            int pivoti = pivots[ki];
            int j;
            for(j=0; j<dim; j++){
                distance += pow(coord[pivoti*dim + j] - coord[i*dim + j] ,2);
            }
            rebuiltCoord[i*k + ki] = sqrt(distance);
        }
    }
	//求距离函数的值 
    // Calculate the sum of Chebyshev distance with rebuilt coordinates between every points
    double chebyshevSum = 0;
    int block=n;
    int l=0,r=0;
   	for(i=0; i<n; i++){
        int j;
        for(j=i+1; j<n; j++){
            double chebyshev = 0;
            int ki;
            for(ki=0; ki<k; ki++){
                double dis = fabs(rebuiltCoord[i*k + ki] - rebuiltCoord[j*k + ki]);
                chebyshev = dis>chebyshev ? dis : chebyshev;
            }
            chebyshevSum += chebyshev*2;
        }
    }
    free(rebuiltCoord);

    return chebyshevSum;
}

// Recursive function Combination() : combine pivots and calculate the sum of distance while combining different pivots.
// ki  : current depth of the recursion
// k   : number of pivots
// n   : number of points
// dim : dimension of metric space
// M   : number of combinations to store
// coord  : coordinates of points
// pivots : indexes of pivots   支撑点的下标 
// maxDistanceSum  : the largest M distance sum  最大的m个距离 
// maxDisSumPivots : the top M pivots combinations   最大的m个支撑点 
// minDistanceSum  : the smallest M distance sum   最小的m个距离 
// minDisSumPivots : the bottom M pivots combinations  最小的m个支撑点 
void Combination(int ki, const int k, const int n, const int dim, const int M, double* coord, int* pivots,
                 double* maxDistanceSum, int* maxDisSumPivots, double* minDistanceSum, int* minDisSumPivots){
    if(ki==k-1){
        int i;
        for(i=pivots[ki-1]+1; i<n; i++){
            pivots[ki] = i;
            // Calculate sum of distance while combining different pivots.
            double distanceSum= SumDistance(k, n, dim, coord, pivots,maxDistanceSum[M-1],minDistanceSum[M-1]);
			int flag1=0,flag2=0;
            // put data at the end of array
            if(maxDistanceSum[M-1]<distanceSum)
                maxDistanceSum[M] = distanceSum,flag1=1;
            if(minDistanceSum[M-1]>distanceSum)
                minDistanceSum[M] = distanceSum,flag2=1;
            int kj;
            for(kj=0; kj<k&&flag1==1; kj++){
                maxDisSumPivots[M*k + kj] = pivots[kj];
            }
            for(kj=0; kj<k&&flag2==1; kj++){
                minDisSumPivots[M*k + kj] = pivots[kj];
            }
            // sort
            int a;
            for(a=M; a>0&&flag1==1; a--){
                if(maxDistanceSum[a] > maxDistanceSum[a-1]){
                    double temp = maxDistanceSum[a];
                    maxDistanceSum[a] = maxDistanceSum[a-1];
                    maxDistanceSum[a-1] = temp;
                    int kj;
                    for(kj=0; kj<k; kj++){
                        int temp = maxDisSumPivots[a*k + kj];
                        maxDisSumPivots[a*k + kj] = maxDisSumPivots[(a-1)*k + kj];
                        maxDisSumPivots[(a-1)*k + kj] = temp;
                    }
                }
            }
            for(a=M; a>0&&flag2==1; a--){
                if(minDistanceSum[a] < minDistanceSum[a-1]){
                    double temp = minDistanceSum[a];
                    minDistanceSum[a] = minDistanceSum[a-1];
                    minDistanceSum[a-1] = temp;
                    int kj;
                    for(kj=0; kj<k; kj++){
                        int temp = minDisSumPivots[a*k + kj];
                        minDisSumPivots[a*k + kj] = minDisSumPivots[(a-1)*k + kj];
                        minDisSumPivots[(a-1)*k + kj] = temp;
                    }
                }
            }
        }
        
        return;
    }
    // Recursively call Combination() to combine pivots
    int i;
    for(i=pivots[ki-1]+1; i<n; i++) {
        pivots[ki] = i;
        Combination(ki+1, k, n, dim, M, coord, pivots, maxDistanceSum, maxDisSumPivots, minDistanceSum, minDisSumPivots);
    }
}

int main(int argc, char* argv[]){
	MPI_Init(&argc, &argv);
    int rank,size;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Status status;
    // M : number of combinations to store
    const int M = 1000;
    // dim : dimension of metric space
    int dim;
	// n : number of points
    int n;
    // k : number of pivots
    int k;
    // Start timing
    struct timeval start;
    // Read parameter
    // filename : input file namespace
    char* filename = (char*)"uniformvector-2dim-5h.txt";
    if( argc==2 ) {
        filename = argv[1];
    }  else if(argc != 1) {
        printf("Usage: ./pivot <filename>\n");
        return -1;
    }
    FILE* file = fopen(filename, "r");
    if( file == NULL ) {
        printf("%s file not found.\n", filename);
        return -1;
    }
    if(rank==0){
        fscanf(file, "%d", &dim);
        fscanf(file, "%d", &n);
        fscanf(file, "%d", &k);
        printf("dim = %d, n = %d, k = %d\n", dim, n, k);
    }
    MPI_Bcast(&dim, 1, MPI_INT,0, MPI_COMM_WORLD);
    MPI_Bcast(&n,1, MPI_INT,0, MPI_COMM_WORLD);
    MPI_Bcast(&k, 1, MPI_INT,0, MPI_COMM_WORLD);
    // Read Data 
	
    double* coord = (double*)malloc(sizeof(double) * dim * n);

    if(rank==0){
        int i;
        for(i=0; i<n; i++){
            int j;
            for(j=0; j<dim; j++){
                fscanf(file, "%lf", &coord[i*dim + j]);
            }
        }
  
    }
    fclose(file);
    gettimeofday(&start, NULL);  
    MPI_Bcast(coord, n*dim, MPI_DOUBLE,0, MPI_COMM_WORLD);
    // maxDistanceSum : the largest M distance sum
    double* maxDistanceSum = (double*)malloc(sizeof(double) * (M+1));
    int i;
    for(i=0; i<M; i++){
        maxDistanceSum[i] = 0;
    }
    // maxDisSumPivots : the top M pivots combinations
    int* maxDisSumPivots = (int*)malloc(sizeof(int) * k * (M+1));
    for(i=0; i<M; i++){
        int ki;
        for(ki=0; ki<k; ki++){
            maxDisSumPivots[i*k + ki] = 0;
        }
    }
    // minDistanceSum : the smallest M distance sum
    double* minDistanceSum = (double*)malloc(sizeof(double) * (M+1));
    for(i=0; i<M; i++){
        minDistanceSum[i] = __DBL_MAX__;
    }
    // minDisSumPivots : the bottom M pivots combinations
    int* minDisSumPivots = (int*)malloc(sizeof(int) * k * (M+1));
    for(i=0; i<M; i++){
        int ki;
        for(ki=0; ki<k; ki++){
            minDisSumPivots[i*k + ki] = 0;
        }
    }
	
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
        // End timing
        struct timeval end;
        gettimeofday (&end, NULL);
        printf("Using time : %f ms\n", (end.tv_sec-start.tv_sec)*1000.0+(end.tv_usec-start.tv_usec)/1000.0);
        // Store the result
        FILE* out = fopen("result.txt", "w");
        for(i=0; i<M; i++){
            int ki;
            for(ki=0; ki<k-1; ki++){
                fprintf(out, "%d ", maxDisSumPivots[i*k + ki]);
            }
            fprintf(out, "%d\n", maxDisSumPivots[i*k + k-1]);
        }
        for(i=0; i<M; i++){
            int ki;
            for(ki=0; ki<k-1; ki++){
                fprintf(out, "%d ", minDisSumPivots[i*k + ki]);
            }
            fprintf(out, "%d\n", minDisSumPivots[i*k + k-1]);
        }
        fclose(out);

        // Log
        int ki;
        printf("max : ");
        for(ki=0; ki<k; ki++){
            printf("%d ", maxDisSumPivots[ki]);
        }
        printf("%lf\n", maxDistanceSum[0]);
        printf("min : ");
        for(ki=0; ki<k; ki++){
            printf("%d ", minDisSumPivots[ki]);
        }
        printf("%lf\n", minDistanceSum[0]);
         
	}
    MPI_Finalize();
    return 0;
}

