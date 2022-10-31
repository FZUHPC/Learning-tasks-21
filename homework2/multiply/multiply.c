#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include<math.h>
#include <immintrin.h>

const int N = 500;
const int thread_count = 10;

double a[500][500];
double b[500][500];
double c_0[500][500];
double c[500][500];

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
			for(int k=1;k<N;k++){
				c_0[i][j] += a[i][k]*b[k][j];
				// printf("%lf %lf\n", a[i][k], b[k][j]);
				// if(k > 50){
				// 	exit(0);
				// }
			}
	
	t1 = omp_get_wtime();
	
	T0 = (t1-t0)*1000;
	printf("%f ms\n", T0);
//---------------------------------------------------//	



	t0 = omp_get_wtime();
	
//-----------优化矩阵乘法运算（需修改部分）-----------// 
//--------------- c = a * b -------------------------//

// #pragma omp parallel for num_threads(thread_count)
// for(int i=1;i<N;i++){
// 	for(int j=1;j<N;j++){
// 		int k = 1;
// 		for(; k <= N-4; k+=4){
// 			__m256d v1 = _mm256_loadu_pd(&a[i][k]);
// 			__m256d v2 = _mm256_set_pd(b[k+3][j], b[k+2][j], b[k+1][j], b[k][j]);
// 			v1 = _mm256_mul_pd(v1, v2);
// 			double* tmp = (double*)&v1;
// 			c[i][j] += tmp[0] + tmp[1] + tmp[2] + tmp[3];
// 		}
// 		for(; k < N; k++){
// 			c[i][j] += a[i][k] * b[k][j];
// 		}
// 	}
// }

#pragma omp parallel for num_threads(thread_count)
for(int i=1;i<N;i++){
	for(int j=1;j<N;j++){
		int k = 1;
		for(; k <= N-24; k+=24){
			__m256d va1 = _mm256_loadu_pd(&a[i][k]);
			__m256d va2 = _mm256_loadu_pd(&a[i][k+4]);
			__m256d va3 = _mm256_loadu_pd(&a[i][k+8]);
			__m256d va4 = _mm256_loadu_pd(&a[i][k+12]);
			__m256d va5 = _mm256_loadu_pd(&a[i][k+16]);
			__m256d va6 = _mm256_loadu_pd(&a[i][k+20]);

			__m256d vb1 = _mm256_set_pd(b[k+3][j], b[k+2][j], b[k+1][j], b[k][j]);
			__m256d vb2 = _mm256_set_pd(b[k+7][j], b[k+6][j], b[k+5][j], b[k+4][j]);
			__m256d vb3 = _mm256_set_pd(b[k+11][j], b[k+10][j], b[k+9][j], b[k+8][j]);
			__m256d vb4 = _mm256_set_pd(b[k+15][j], b[k+14][j], b[k+13][j], b[k+12][j]);
			__m256d vb5 = _mm256_set_pd(b[k+19][j], b[k+18][j], b[k+17][j], b[k+16][j]);
			__m256d vb6 = _mm256_set_pd(b[k+23][j], b[k+22][j], b[k+21][j], b[k+20][j]);

			__m256d v1 = _mm256_mul_pd(va1, vb1);
			__m256d v2 = _mm256_mul_pd(va2, vb2);
			__m256d v3 = _mm256_mul_pd(va3, vb3);
			__m256d v4 = _mm256_mul_pd(va4, vb4);
			__m256d v5 = _mm256_mul_pd(va5, vb5);
			__m256d v6 = _mm256_mul_pd(va6, vb6);

			v1 = _mm256_add_pd(v1, v2);
			v2 = _mm256_add_pd(v3, v4);
			v3 = _mm256_add_pd(v5, v6);
			v1 = _mm256_add_pd(v1, v2);
			v1 = _mm256_add_pd(v1, v3);

			v1 = _mm256_hadd_pd(v1, _mm256_setzero_pd());


			c[i][j] += v1[0] + v1[2];
		}
		for(; k < N; k++){
			c[i][j] += a[i][k] * b[k][j];
		}
	}
}

//--------------------------------------------------//	

	t1 = omp_get_wtime();
	
	T1 = (t1-t0)*1000;
	printf("%f ms\n", T1);	

	printf("speed up: %f\n",T0/T1);

//----------------数据输出---------------------------// 
	for(int i=1;i<N;i++)
		for(int j=1;j<N;j++)
			fprintf(out,"%lf\n",c[i][j]);
//---------------------------------------------------//			
			
	fclose(f1);
	fclose(f2);
	fclose(out);			
}

