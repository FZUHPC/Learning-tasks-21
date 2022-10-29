#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdarg.h>
#include <immintrin.h>

const int test_row = 1000;
const int test_col = 1000;
const int thread_count = 10;

typedef struct{
    int row;
    int col;
    double** m;
}Matrix;

struct params{
    int tid;
    Matrix* res;
    Matrix* m1;
    Matrix* m2;
};

void matrix_init(Matrix* m, int row, int col){
    m->row = row;
    m->col = col;
    m->m = (double**)malloc(m->row * sizeof(double*));
    for(int r=0; r<m->row; r++){
        m->m[r] = (double*)calloc(m->col, sizeof(double));
    }
}

void matrix_set(Matrix* m, ...){
    va_list v;
    va_start(v, m);
    double value;
    for(int r=0; r<m->row; r++){
        for(int c=0; c<m->col; c++){
            value = va_arg(v, double);
            m->m[r][c] = value;
        }
    }
    va_end(v);
}

void matrix_set_test(Matrix* m){
    for(int r=0; r<m->row; r++){
        for(int c=0; c<m->col; c++){
            m->m[r][c] = 2;
        }
    }
}

void matrix_print(Matrix* m){
    for(int r=0; r<m->row; r++){
        for(int c=0; c<m->col; c++){
            printf("%.3lf ", m->m[r][c]);
        }
        printf("\n");
    }
}

void matrix_destroy(Matrix* m){
    for(int r=0; r<m->row; r++){
        free(m->m[r]);
    }
    free(m->m);
}

void matrix_multiply_normal(Matrix* res, Matrix* m1, Matrix* m2){
    if(m1->col == m2->row && res->row == m1->row && res->col == m2->col){
        for(int r=0; r < res->row; r++){
            for(int c=0; c < res->col; c++){
                res->m[r][c] = 0;
                for(int i=0; i< m1->col; i++){
                    res->m[r][c] += m1->m[r][i] * m2->m[i][c];
                }
            }
        }
    }
}

void* __matrix_multiply_divider(void* args){
    struct params* p = (struct params*)args;
    int each = p->res->row / thread_count;
    int start = p->tid * each;
    int end = p->tid == thread_count-1? p->m1->row-1: (p->tid + 1) * each -1;
    for(int r=start; r<=end; r++){
        for(int c=0; c<p->res->col; c++){
            p->res->m[r][c] = 0;
            for(int i=0; i<p->m1->col; i++){
                p->res->m[r][c] += p->m1->m[r][i] * p->m2->m[i][c];
            }
        }
    }
    free(p);
    return NULL;
}
void matrix_multiply_pthread(Matrix* res, Matrix* m1, Matrix* m2){
    if(m1->col == m2->row && res->row == m1->row && res->col == m2->col){
        pthread_t* thread_handles = (pthread_t*)malloc(thread_count * sizeof(pthread_t*));
        for(int tid=0; tid<thread_count; tid++){
            struct params* p = (struct params*)malloc(sizeof(struct params));
            p->tid = tid;
            p->res = res;
            p->m1 = m1;
            p->m2 = m2;
            pthread_create(&thread_handles[tid], NULL, __matrix_multiply_divider, (void*)p);
        }
        for(int tid=0; tid<thread_count; tid++){
            pthread_join(thread_handles[tid], NULL);
        }
        free(thread_handles);
    }
}

void matrix_multiply_simd(Matrix* res, Matrix* m1, Matrix* m2){
    if(m1->col == m2->row && res->row == m1->row && res->col == m2->col){
        for(int r=0; r < res->row; r++){
            for(int c=0; c < res->col; c++){
                res->m[r][c] = 0;
                int i;
                for(i=0; i< m1->col - 4; i+=4){
                    __m256d v1 = _mm256_set_pd(m1->m[r][i], m1->m[r][i+1], m1->m[r][i+2], m1->m[r][i+3]);
                    __m256d v2 = _mm256_set_pd(m2->m[i][c], m2->m[i+1][c], m2->m[i+2][c], m2->m[i+3][c]);
                    __m256d v = _mm256_mul_pd(v1, v2);
                    double* tmp = (double*)&v;
                    res->m[r][c] += tmp[0] + tmp[1] + tmp[2] + tmp[3];
                }
                for(; i< m1->col; i++){
                    res->m[r][c] += m1->m[r][i] * m2->m[i][c];
                }
            }
        }
    }
}

void matrix_multiply_omp(Matrix* res, Matrix* m1, Matrix* m2){
    if(m1->col == m2->row && res->row == m1->row && res->col == m2->col){
        for(int r=0; r < res->row; r++){
            #pragma omp parallel for num_threads(thread_count)
            for(int c=0; c < res->col; c++){
                res->m[r][c] = 0;
                for(int i=0; i< m1->col; i++){
                    res->m[r][c] += m1->m[r][i] * m2->m[i][c];
                }
            }
        }
    }
}

void check(Matrix* res){
    for(int r=0; r< res->row; r++){
        for(int c=0; c< res->col; c++){
            if(res->m[r][c] != 4000){
                printf("err\n");
            }
        }
    }
}

int main(int argc, char** argv){
    Matrix m1, m2, res;
    matrix_init(&m1, test_row, test_col);
    matrix_init(&m2, test_row, test_col);
    matrix_init(&res, test_row, test_col);

    matrix_set_test(&m1);
    matrix_set_test(&m2);

    if(argc == 2){
        if(strcmp(argv[1], "omp") == 0){
            matrix_multiply_omp(&res, &m1, &m2);
        }else if(strcmp(argv[1], "simd") == 0){
            matrix_multiply_simd(&res, &m1, &m2);
        }else if(strcmp(argv[1], "pth") == 0){
            matrix_multiply_pthread(&res, &m1, &m2);
        }else if(strcmp(argv[1], "normal") == 0){
            matrix_multiply_normal(&res, &m1, &m2);
        }
    }else{
        printf("please choose a mode: \n");
        printf("  omp\n");
        printf("  simd\n");
        printf("  pth\n");
        printf("  normal\n");
    }

    matrix_destroy(&m1);
    matrix_destroy(&m2);
    matrix_destroy(&res);
}