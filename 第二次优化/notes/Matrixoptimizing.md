- ## 矩阵加减乘除算法

**<u>加减乘是比较好完成的，但是除法比较繁琐，将除法转换为乘法，除一个矩阵等于乘以它的逆矩阵。也就是说还要去编写算法得出它的逆矩阵，而逆矩阵又可以通过伴随矩阵除以行列式而得到。</u>**

```cpp
#include<stdio.h>
#include<string.h>
#include<bits/stdc++.h>
#include<cstdio>
using namespace std;
#define N 100
int row1,column1,row2,column2;
void input(double *a,double *b,double *c)	//输入A，B两个矩阵
{
	for (int i = 0; i <= row1 - 1; i++) {
		for (int j = 0; j <= column1 - 1; j++) {
			scanf("%lf", &a[i*column1+j]);
			}
		}
	for (int i = 0; i <= row2 - 1; i++) {
		for (int j = 0; j <= column2 - 1; j++) {
			scanf("%lf", &b[i*column2+j]);
			}
		}	
	for (int i = 0; i <= row1 - 1; i++) {
		for (int j = 0; j <= column2 - 1; j++) {
			c[i*column2+j] = 0;
			}
		}
 		//将矩阵（二维数组）C 的所有元素全部初始化为零
}

void multiplication(double *a,double *b,double *c)
{
		//矩阵乘法
	for (int i = 0; i <= row1 - 1; i++) {
		for (int j = 0; j <= column2 - 1; j++) {
			for (int k = 0; k <= column1 - 1; k++) {
				c[i*column2+j] += a[i*column1+k] * b[k*column2+j];
				}
			}
		}
	
}

void addition(double *a,double *b,double *c) 
{
	for (int i = 0; i <= row1 - 1; i++) {
		for (int j = 0; j <= column2 - 1; j++) {
				c[i*column2+j] = a[i*column1+j] + b[i*column2+j];
			}
		}
	
}

void subtraction(double *a,double *b,double *c) 
{
	for (int i = 0; i <= row1 - 1; i++) {
		for (int j = 0; j <= column2 - 1; j++) {
				c[i*column2+j] = a[i*column1+j] - b[i*column2+j];
			}
		}
	
}
 

 
void print(double *a,double *b,double *c)
{	//输出结果
	for (int i = 0; i <= row1 - 1; i++) {
		for (int j = 0; j <= column2 - 1; j++) {
			printf("%10.5f ", c[i*column2+j]);
			}
			printf("\n");
		}
}
 

double getA(double arcs[N][N],int n)//按第一行展开计算|A|
{
	if(n==1)
	{
		return arcs[0][0];
	}
	double ans = 0;
	double temp[N][N];
	int i,j,k;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n-1;j++)
		{
			for(k=0;k<n-1;k++)
			{
				temp[j][k] = arcs[j+1][(k>=i)?k+1:k];
				
			}
		}
		double t = getA(temp,n-1);
		if(i%2==0)
		{
			ans += arcs[0][i]*t;
		}
		else
		{
			ans -=  arcs[0][i]*t;
		}
	}
	return ans;
}
void getAStart(double arcs[N][N],int n,double ans[N][N])//计算每一行每一列的每个元素所对应的余子式，组成A*
{
	if(n==1)
	{
		ans[0][0] = 1;
		return;
	}
	int i,j,k,t;
	double temp[N][N];
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			for(k=0;k<n-1;k++)
			{
				for(t=0;t<n-1;t++)
				{
					temp[k][t] = arcs[k>=i?k+1:k][t>=j?t+1:t];
				}
			}
 
		
			ans[j][i]  =  getA(temp,n-1);
			if((i+j)%2 == 1)
			{
				ans[j][i] = - ans[j][i];
			}
		}
	}
}
 
void division(void)
{
    double arcs[N][N];
	double astar[N][N];
	double reverse[N][N];
	double a[N][N],c[N][N];
	int i,j;
	int n,r1,c1;
	
	
	scanf("%d %d %d",&r1,&c1,&n);
	if(c1!=n)
	{
		printf("can not divide!\n");
			return ;
	}
	for(i=0;i<r1;i++)
		{
			for(j=0;j<c1;j++)
			{
				scanf("%lf",&a[i][j]);
			}
		}
	
		for(i=0;i<n;i++)
		{
			for(j=0;j<n;j++)
			{
				scanf("%lf",&arcs[i][j]);
			}
		}
	
		double result = getA(arcs,n);
		if(result==0)
		{
			printf("can not divide!\n");
			return ;
		}
		else
		{
			getAStart(arcs,n,astar);
			for(i=0;i<n;i++)
			{
				for(j=0;j<n;j++)
				{
					reverse[i][j]=astar[i][j]/result;
				}

			}
		}

	for (int i = 0; i <= r1 - 1; i++) {
		for (int j = 0; j <= n - 1; j++) {
			for (int k = 0; k <= n - 1; k++) {
				c[i][j] += a[i][k] * reverse[k][j];
				}
			}
		}
	
	for (int i = 0; i <= r1 - 1; i++) {
		for (int j = 0; j <= n - 1; j++) {
			printf("%10.5f ", c[i][j]);
			}
			printf("\n");
		}
 
	return ;
}



int main(void)
{
	
	string d;
 	cin>>d;//输入运算类型
 	
 	if(d=="division")
 	{
 		division();
 		return 0;
	}
	
	scanf("%d %d %d %d",&row1,&column1,&row2,&column2);//先输入两个矩阵的行和列 
	const int R1=row1,R2=row2,C1=column1,C2=column2;	//注意数组的声明要用常数
	double A[R1][C1], B[R2][C2], C[R1][C2];
	
 	input(A[R1],B[R2],C[R1]);//输入两个矩阵 
 	 
 	if(d=="multiplication")
 	{
	 	if(R2!=C1)
		{
			printf("ERROR");
			return 0; 
		}	
		else 
	 		{multiplication(A[R1],B[R2],C[R1]);} 
	}
	
	if(d=="addition")
	{
		if(R1!=R2||C1!=C2)
		{
			printf("ERROR");
			return 0; 
		}	
		else
			addition(A[R1],B[R2],C[R1]);
	}
	 	
	if(d=="subtraction")
	{
		if(R1!=R2||C1!=C2)
		{
			printf("ERROR");
			return 0; 
		}	
		else
			subtraction(A[R1],B[R2],C[R1]);
	}	

	print(A[R1],B[R2],C[R1]);
 
	return 0;
}
```

#### **之后将以矩阵的乘法为核心，进行之后的优化。**

代码如下：

```cpp
#include <stdio.h>
#include<string.h>
#include<bits/stdc++.h>
#include<cstdio>
using namespace std;
#define M 3
#define K 2
#define N 3
int row1,column1,row2,column2;
double Matrix_A[M][K] = {{1.56,4.67},{2.78,5.89},{3.98,6.34}};
double Matrix_B[K][N] = {{8.19,7.22,6.44},{5.53,4.67,3.76}};
double Matrix_C[M][N];
 
 
int main(void)
{
	int i,j;
	for (int i = 0; i <= M - 1; i++) {
		for (int j = 0; j <= N - 1; j++) {
			Matrix_C[i][j]=0;
			for (int k = 0; k <= K - 1; k++) {
			 Matrix_C[i][j] += Matrix_A[i][k] * Matrix_B[k][j];
				}			 
				printf("%10.5lf",Matrix_C[i][j]);
			}
			printf("\n");
		}
 
	return 0;
}
```

## 测试

#### perf的使用

<u>由于前面的vtune的使用一直有问题，进行分析时一直显示collection failed，所以就先尝试使用了一下Linux自带的性能分析工具Perf</u>

**以下就说明一下perf的几个简单的应用**

1. `perf --help`  

	打印Perf命令可用的所有选项

2. `perf top` 

	可以将top子命令与Perf一起使用以实时显示性能计数器概要文件：例如，将perf top命令与-a选项一起使用可查看所有已知事件类型。

	`perf top -a`

	要显示与CPU时钟相关的事件，请使用-e选项

	`perf top -e cpu-clock`

3. `perf stat`

	> 分析系统/进程的整体性能概况
	>
	> task‐clock事件表示目标任务真正占用处理器的时间，单位是毫秒。也称任务执行时间
	>
	> context-switches是系统发生上下文切换的次数
	>
	> CPU-migrations是任务从一个处理器迁往另外一个处理器的次数
	>
	> page-faults是内核发生缺页的次数
	>
	> cycles是程序消耗的处理器周期数
	>
	> instructions是指命令执行期间产生的处理器指令数
	>
	> branches是指程序在执行期间遇到的分支指令数。
	>
	> branch‐misses是预测错误的分支指令数。
	>
	> XXX seconds time elapsed系程序持续时间
	>
	> 任务执行时间/任务持续时间大于1，那可以肯定是多核引起的

4. `Perf List`
	利用perf剖析程序性能时，需要指定当前测试的性能时间。性能事件是指在处理器或操作系统中发生的，可能影响到程序性能的硬件事件或软件事件

5. `Perf record`
	记录一段时间内系统/进程的性能时间

6. `Perf Report`
	读取perf record生成的数据文件，并显示分析数据

#### posix thread的运用

**在Linux编写多线程程序需要包含头文件**

`＃include <pthread.h> `

**在Linux下创建的线程的API接口是pthread_create()，它的完整定义是：**

`int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg); `

> int pthread_create (pthread_t*  thread_p ,
> const pthread_attr_t*  attr_p ,
> void*  (*start_routine ) ( void ) ,
> void*  arg_p ) ;
> //第一个参数是一个指针，指向对应的 pthread_t 对象。
> //第二个参数一般用 NULL 就行
> //第三个参数表示该线程将要运行的函数。
> //最后一个参数也是一个指针，指向传给函数 start_routine 的参数列表。
>
> 函数参数： 
> \1. 线程句柄 thread：当一个新的线程调用成功之后，就会通过这个参数将线程的句柄返回给调用者，以便对这个线程进行管理。 
> \2. 入口函数 start_routine()： 当你的程序调用了这个接口之后，就会产生一个线程，而这个线程的入口函数就是start_routine()。如果线程创建成功，这个接口会返回0。 
> \3. 入口函数参数 *arg : `start_routine()`函数有一个参数，这个参数就是`pthread_create`的最后一个参数arg。这种设计可以在线程创建之前就帮它准备好一些专有数据，最典型的用法就是使用C++编程时的`this`指针。`start_routine()`有一个返回值，这个返回值可以通过`pthread_join()`接口获得。 
> \4. 线程属性 attr： `pthread_create()`接口的第二个参数用于设置线程的属性。这个参数是可选的，当不需要修改线程的默认属性时，给它传递NULL就行。

**在很多情况下，主线程生成并起动了子线程，如果子线程里要进行大量的耗时的运算，主线程往往将于子线程之前结束，但是如果主线程处理完其他的事务后，需要用到子线程的处理结果，也就是主线程需要等待子线程执行完成之后再结束，这个时候就要用到pthread_join()方法了。**
**即pthread_join()的作用可以这样理解：主线程等待子线程的终止。也就是在子线程调用了pthread_join()方法后面的代码，只有等到子线程结束了才能执行。**

```c
int pthread_join(pthread_t thread, void **retval);
args:
    pthread_t thread: 被连接线程的线程号
    void **retval : 指向一个指向被连接线程的返回码的指针的指针
return:
    线程连接的状态，0是成功，非0是失败
```

**Posix线程中的线程属性pthread_attr_t主要包括 scope 属性、detach属性、堆栈地址、堆栈大**
**小、优先级。在pthread_create中，把第二个参数设置为NULL的话，将采用默认的属性配置。**
**线程属性结构如下：**

```c++
typedef struct
{
       int                       detachstate;     线程的分离状态
       int                       schedpolicy;     线程调度策略
       struct sched_param        schedparam;      线程的调度参数
       int                       inheritsched;    线程的继承性
       int                       scope;           线程的作用域
       size_t                    guardsize;       线程栈末尾的警戒缓冲区大小
       int                       stackaddr_set;
       void *                    stackaddr;       线程栈的位置
       size_t                    stacksize;       线程栈的大小
}pthread_attr_t;
```

主要用到的函数如下：

1. `pthread_attr_init`
	功能： 对线程属性变量的初始化。
	头文件 ： <pthread.h>
	函数原型： int pthread_attr_init (pthread_attr_t* attr);
	函数传入值：attr:线程属性。
	函数返回值：成功： 0
	失败： -1
2. `pthread_attr_setscope`
	功能： 设置线程 __scope 属性。
	头文件： <pthread.h>
	函数原型： int pthread_attr_setscope (pthread_attr_t* attr, int scope);
	函数传入值：attr: 线程属性。
	scope:PTHREAD_SCOPE_SYSTEM，表示与系统中所有线程一起竞争CPU时间，
	PTHREAD_SCOPE_PROCESS，表示仅与同进程中的线程竞争CPU
	函数返回值得：同1。
3. `pthread_attr_setdetachstate`
	功能： 设置线程detachstate属性。
	头文件： <phread.h>
	函数原型： int pthread_attr_setdetachstate (pthread_attr_t* attr, int detachstate);
	函数传入值：attr:线程属性。
	detachstate:PTHREAD_CREATE_DETACHED，不能用pthread_join()来同步，且在退出时自行释
	放所占用的资源
	PTHREAD_CREATE_JOINABLE，能用pthread_join()来同步
	函数返回值得：同1。
4. `pthread_attr_setschedparam`
	功能： 设置线程schedparam属性，即调用的优先级。
	头文件： <pthread.h>
	函数原型： int pthread_attr_setschedparam (pthread_attr_t* attr, struct sched_param* param);
	函数传入值：attr：线程属性。
	param：线程优先级。一个struct sched_param结构，目前仅有一个sched_priority整型变量表示线
	程的运行优先级。这个参数仅当调度策略为实时（即SCHED_RR或SCHED_FIFO）时才有效，并
	可以在运行时通过pthread_setschedparam()函数来改变，缺省为0
	函数返回值：同1。
5. `pthread_attr_getschedparam`
	功能： 得到线程优先级。
	头文件： <pthread.h>
	函数原型： int pthread_attr_getschedparam (pthread_attr_t* attr, struct sched_param* param);
	函数传入值：attr：线程属性；
	param：线程优先级；
	函数返回值：同1。

### 下面是我通过pthread编写2种的矩阵乘法：

```c++
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define M 3
#define K 2
#define N 3
typedef struct{
    int row,
		column;
}parameter;
double Matrix_A[M][K] = {{1.56,4.67},{2.78,5.89},{3.98,6.34}};
double Matrix_B[K][N] = {{8.19,7.22,6.44},{5.53,4.67,3.76}};
double Matrix_C[M][N];
void* worker(void* para){
    parameter* par = (parameter*)para;
    int i,sum = 0;
    for(i = 0;i < K;i++)    
		Matrix_C[par->row][par->column] += Matrix_A[par->row][i] * Matrix_B[i][par->column];
	printf("%10.5lf",Matrix_C[par->row][par->column]);
//    printf("POSIX Thread: %lu :C[%d][%d] = %d\n",
//            pthread_self(),par->row,par->column,sum);
    pthread_exit(0);
}
int main(){
    int i,j;
    pthread_t workers[M][N];
//    pthread_attr_t attr;
    /* get the default attributes */
//    pthread_attr_init(&attr);
    for(i = 0;i < M;i++){
        for(j = 0;j < N;j++){
            parameter* para = (parameter*)malloc(sizeof(parameter));
            para->row = i;
            para->column = j;
            /* create the thread*/
            pthread_create(&workers[i][j],NULL,worker,para);
            // pthread_create(&workers[i][j],&attr,worker,para);
            pthread_join(workers[i][j],NULL);
            free(para);
//            sleep(1);
        }
        printf("\n");
    }
    return 0;
}
```

```c
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define M 3
#define K 2
#define N 3
typedef struct{
    int row,
		column;
}parameter;
double Matrix_A[M][K] = {{1.56,4.67},{2.78,5.89},{3.98,6.34}};
double Matrix_B[K][N] = {{8.19,7.22,6.44},{5.53,4.67,3.76}};
double Matrix_C[M][N];
void* worker(void* para){
    parameter* par = (parameter*)para;
    int i,sum = 0;
    for(int j = 0;j < N;j++){
		par->column = j;
		for(i = 0;i < K;i++)    
			Matrix_C[par->row][par->column] += Matrix_A[par->row][i] * Matrix_B[i][par->column];
		printf("%10.5lf",Matrix_C[par->row][par->column]);
	}
//    printf("POSIX Thread: %lu :C[%d][%d] = %d\n",
//            pthread_self(),par->row,par->column,sum);
    pthread_exit(0);
}
int main(){
    int i,j;
    pthread_t workers[M];
//    pthread_attr_t attr;
    /* get the default attributes */
//    pthread_attr_init(&attr);
    for(i = 0;i < M;i++){
        
            parameter* para = (parameter*)malloc(sizeof(parameter));
            para->row = i;
            
            /* create the thread*/
            pthread_create(&workers[i],NULL,worker,para);
            // pthread_create(&workers[i][j],&attr,worker,para);
            pthread_join(workers[i],NULL);
            free(para);
//            sleep(1);
        
        printf("\n");
    }
    return 0;
}
```

> 第一种是将结果的矩阵每个结点作为一个线程，第二种则是将每个矩阵行作为一个线程，两者的线程数不相同，而API接口中的入口函数也不同，两种方法都能加速原来的矩阵算法。其中第二种加速较多，第一种由于线程太多，切换线程耗时较多，导致程序完成时间同优化前相差不多。

![mutimatrix](D:\我的文件\实验室\第二次\mutimatrix.png)

![pmutimatrix](D:\我的文件\实验室\第二次\pmutimatrix.png)

![pmutimatrix1](D:\我的文件\实验室\第二次\pmutimatrix1.png)

## 向量化操作

```c
#include <x86intrin.h>
#include <stddef.h>
#include <stdio.h>
#define M 3
#define K 2
#define N 3
double Matrix_A[M][K] = {{1.56,4.67},{2.78,5.89},{3.98,6.34}};
double Matrix_B[K][N] = {{8.19,7.22,6.44},{5.53,4.67,3.76}};
double Matrix_C[M][N];
void dgemm_avx( double *A, double *B, double *C)
{
        for (size_t i = 0; i < M; i += 4) {
                for (size_t j = 0; j < N; j++) {
                        __m256d c0 = _mm256_load_pd(C+i+j*N); /* c0 = C[i][j] */
                        for (size_t k = 0; k < K; k++) {
                                c0 = _mm256_add_pd(c0,
                                        _mm256_mul_pd(_mm256_load_pd(A+k+i*K),
                                                _mm256_broadcast_sd(B+j+k*N)));
                        }
                        _mm256_store_pd(C+i+j*N, c0);  /* C[i][j] = c0 */;
                        printf("%10.5lf",C[i+j*N]);
                }
        }
}
int main()
{
        dgemm_avx(Matrix_A[M],Matrix_B[K],Matrix_C[M]);
        return 0;
}

```

详情参考：[向量化指令](https://www.laruence.com/sse/#techs=AVX,AVX2)

## shell脚本，实现对后台CPU使用率的的监控

```shell
#!/bin/bash
now=`date -u -d"+8 hour" +'%Y-%m-%d %H:%M:%S'`
#cpu报警阈值
cpu_warn='75'
#主代码
item_cpu () {
cpu_idle=`top -b -n 1 | grep Cpu | awk '{printf $8}'|cut -f 1 -d "."`
cpu_use=`expr 100 - $cpu_idle`
echo "$now 当前cpu使用率为 $cpu_use" >> /opt/cpu.log
        if [ $cpu_use -gt $cpu_warn ]
        then
                echo "cpu warning!"
        else
                echo "cpu ok!"
        fi
cpu_used=$(top -b -n1 | head -5 | grep ^%Cpu | awk '{printf("%.2f%%     "),100-$8}')    #已使用CPU百分比

top_proc_cpu=$(ps --no-headers -eo comm,pcpu | sort -k2 -n | tail -5)     #占用cpu最多的5个进程
echo -e "已使用CPU百分比：\033[32m$cpu_used\033[0m"
echo -e "占用CPU资源最多的5个进程列表：">>/opt/cpu.log
echo -e "$top_proc_cpu">>/opt/cpu.log
}
item_cpu
```

**此脚本还能将CPU利用率前五的进程信息存储到文件中**