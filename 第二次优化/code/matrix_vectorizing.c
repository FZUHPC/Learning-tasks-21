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
