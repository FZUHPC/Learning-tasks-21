#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>
#include <immintrin.h>
#include <x86intrin.h>
#include <threads.h>

#define MAXN 497 // 这里增加一个MAXN 方便修改

const int N = MAXN;

double a[MAXN][MAXN];
double b[MAXN][MAXN];
double c_0[MAXN][MAXN];
double c[MAXN][MAXN];

int start_prefix = 1;

void multi_thread_solve(int *p)
{
	int core = *p

	for(int j=1;j<N;j++)
			for(int k=1;k<N;k++)
				c[core][j] += a[core][k]*b[k][j];
}

int main(){

//---------------读取数据---------------------------//
	FILE* f1;
	FILE* f2;
	FILE* out;

	f1 = fopen("data_a.txt", "r");
	f2 = fopen("data_b.txt", "r");
	out = fopen("data_c.txt", "w");

	for(int i=1;i<N;i++)
		for(int j=1;j<N;j++){
			fscanf(f1,"%lf",&a[i][j]);
			fscanf(f2,"%lf",&b[i][j]);
		}
//--------------------------------------------------//

	double t0,t1;
	double T0,T1;


//--------------初始矩阵乘法运算--------------------//
	t0 = omp_get_wtime();
	//mul
	for(int i=1;i<N;i++)
		for(int j=1;j<N;j++)
			for(int k=1;k<N;k++)
				c_0[i][j] += a[i][k]*b[k][j];

	t1 = omp_get_wtime();

	T0 = (t1-t0)*1000;
	printf("初始矩阵乘法运行耗时: %f ms\n", T0);
//---------------------------------------------------//
	t0 = omp_get_wtime();

//-----------优化矩阵乘法运算（需修改部分）-----------//
//--------------- c = a * b -------------------------//

	thrd_t thid[N+10];
	for(int i=1;i<N;i++)
	{
		thrd_create(&thid[i], (thrd_start_t)multi_thread_solve,i);
	}

	for(int i=1;i<N;i++)
	{
		thrd_join(thid[i],NULL);
	}

//--------------------------------------------------/

	t1 = omp_get_wtime();

	T1 = (t1-t0)*1000;
	printf("优化后矩阵乘法运行耗时: %f ms\n", T1);

	printf("加速比为：%f\n",T0/T1);

//----------------数据输出---------------------------//
	for(int i=1;i<N;i++)
		for(int j=1;j<N;j++)
			fprintf(out,"%lf\n",c[i][j]);
//---------------------------------------------------//

	fclose(f1);
	fclose(f2);
	fclose(out);
}
