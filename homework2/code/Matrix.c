#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <immintrin.h>

const int thread_count = 10;
const int row = 1000;
const int col = 1000;

float* matrix1; 
float* matrix2;
float* plus_res;
float* minus_res;
float* multiply_res;

void check_plus();
void check_minus();
void check_multiply();

void matrix_init();
void matrix_destroy();

void matrix_minus_normal();
void matrix_plus_normal();
void matrix_multiply_normal();
    
void matrix_plus_simd();
void matrix_minus_simd();
void matrix_multiply_simd();

void matrix_plus_omp();
void matrix_minus_omp();
void matrix_multiply_omp();

void matrix_plus_pth();
void matrix_minus_pth();
void matrix_multiply_pth();
void* __matrix_plus_pth_divider();
void* __matrix_minus_pth_divider();
void* __matrix_multiply_pth_divider();

int main(int argc, char** argv){
    if(argc == 2){
        matrix_init();
        if(strcmp(argv[1], "normal") == 0){
            // normal
            matrix_minus_normal();
            matrix_plus_normal();
            matrix_multiply_normal();
        }else if(strcmp(argv[1], "simd") == 0){        
            // simd
            matrix_plus_simd();
            matrix_minus_simd();
            matrix_multiply_simd();
        }else if(strcmp(argv[1], "omp") == 0){
            // omp
            matrix_plus_omp();
            matrix_minus_omp();
            matrix_multiply_omp();
        }else if(strcmp(argv[1], "pth") == 0){
            // pth
            matrix_plus_pth();
            matrix_minus_pth();
            matrix_multiply_pth();
        }
        
        // check_plus();
        // check_minus();
        // check_multiply();

        matrix_destroy();
    }else{
        printf("please choose a mode:\n");
        printf("  normal\n");
        printf("  simd\n");
        printf("  omp\n");
        printf("  pth\n");
    }
}


void check_plus(){
    int err = 0;
    for(int r=0; r< row; r++){
        for(int c=0; c< col; c++){
            if(matrix1[r * col + c] == 2 && matrix2[r * col + c] == 2 && plus_res[r * col + c] != 4){
                err = 1;
                break;
            }
        }
        if(err){
            break;
        }
    }
    if(err){
        printf("plus err\n");
    }else{
        printf("plus right\n");
    }
}
void check_minus(){
    int err = 0;
    for(int r=0; r< row; r++){
        for(int c=0; c< col; c++){
            if(matrix1[r * col + c] == 2 && matrix2[r * col + c] == 2 && minus_res[r * col + c] != 0){
                err = 1;
                break;
            }
        }
        if(err){
            break;
        }
    }
    if(err){
        printf("minus err\n");
    }else{
        printf("minus right\n");
    }
}
void check_multiply(){
    int err = 0;
    for(int r=0; r< row; r++){
        for(int c=0; c< col; c++){
            if(matrix1[r * col + c] == 2 && matrix2[r * col + c] == 2 && multiply_res[r * col + c] != 4000){
                err = 1;
                break;
            }
        }
        if(err){
            break;
        }
    }
    if(err){
        printf("multiply err\n");
    }else{
        printf("multiply right\n");
    }
}

void matrix_init(){
    matrix1 = (float*)malloc(row * col * sizeof(float));
    matrix2 = (float*)malloc(row * col * sizeof(float));
    plus_res = (float*)malloc(row * col * sizeof(float));
    minus_res = (float*)malloc(row * col * sizeof(float));
    multiply_res = (float*)malloc(row * col * sizeof(float));
    for(int i=0; i< row*col; i++){
        matrix1[i] = matrix2[i] = plus_res[i] = minus_res[i] = multiply_res[i] = 2;
    }
}
void matrix_destroy(){
    free(matrix1);
    free(matrix2);
    free(plus_res);
    free(minus_res);
    free(multiply_res);
}

void matrix_plus_normal(){
    for(int i=0; i< row * col; i++){
        plus_res[i] = matrix1[i] + matrix1[i];
    }
}
void matrix_minus_normal(){
    for(int i=0; i< row * col; i++){
        minus_res[i] = matrix1[i] - matrix1[i];
    }
}
void matrix_multiply_normal(){
    for(int r=0; r < row; r++){
        for(int c=0; c < col; c++){
            multiply_res[r * col + c] = 0;
            for(int i=0; i < col; i++){
                multiply_res[r * col + c] += matrix1[r * col + i] * matrix2[i * col + c];
            }
        }
    }
}

void matrix_plus_simd(){
    int i = 0;
    int end = row * col - 8;
    for(; i< end; i += 8){
            __m256 v1 = _mm256_loadu_ps(matrix1 + i);
            __m256 v2 = _mm256_loadu_ps(matrix2 + i);
            __m256 v = _mm256_add_ps(v1, v2);
            _mm256_storeu_ps(plus_res + i, v);
    }
    for(; i< row * col; i++){
        plus_res[i] = matrix1[i] + matrix2[i];
    }
}
void matrix_minus_simd(){
    int i = 0;
    int end = row * col - 8;
    for(; i< end; i += 8){
            __m256 v1 = _mm256_loadu_ps(matrix1 + i);
            __m256 v2 = _mm256_loadu_ps(matrix2 + i);
            __m256 v = _mm256_sub_ps(v1, v2);
            _mm256_storeu_ps(minus_res + i, v);
    }
    for(; i< row * col; i++){
        minus_res[i] = matrix1[i] - matrix2[i];
    }
}
void matrix_multiply_simd(){
    for(int r=0; r < row; r++){
        for(int c=0; c < col; c++){
            multiply_res[r * col + c] = 0;
            int i;
            for(i=0; i < col-8; i+=8){
                __m256 v1 = _mm256_loadu_ps(matrix1 + r * col + i);
                __m256 v2 = _mm256_set_ps(matrix2[i * col + c], matrix2[(i+1) * col + c], matrix2[(i+2) * col + c], matrix2[(i+3) * col + c],
                                          matrix2[(i+4) * col + c], matrix2[(i+5) * col + c], matrix2[(i+6) * col + c], matrix2[(i+7) * col + c]);
                __m256 v = _mm256_mul_ps(v1, v2);
                float* tmp = (float*)&v;
                multiply_res[r * col + c] += tmp[0] + tmp[1] + tmp[2] + tmp[3] + tmp[4] + tmp[5] + tmp[6] + tmp[7];
            }
            for(; i < col; i++){
                multiply_res[r * col + c] += matrix1[r * col + i] * matrix2[i * col + c];
            }
        }
    }    
}

void matrix_plus_omp(){
    #pragma omp parallel for num_threads(thread_count)
    for(int i=0; i< row * col; i++){
        plus_res[i] = matrix1[i] + matrix1[i];
    }
}
void matrix_minus_omp(){
    #pragma omp parallel for num_threads(thread_count)
    for(int i=0; i< row * col; i++){
        minus_res[i] = matrix1[i] - matrix1[i];
    }
}
void matrix_multiply_omp(){
    for(int r=0; r < row; r++){
        #pragma omp parallel for num_threads(thread_count)
        for(int c=0; c < col; c++){
            multiply_res[r * col + c] = 0;
            for(int i=0; i < col; i++){
                multiply_res[r * col + c] += matrix1[r * col + i] * matrix2[i * col + c];
            }
        }
    }
}

void* __matrix_plus_pth_divider(void* tid){
    int each = row / thread_count;
    int start = (long)tid * each;
    int end = (long)tid == thread_count-1? row-1: ((long)tid + 1) * each -1;
    for(int r=start; r<=end; r++){
        for(int c=0; c<col; c++){
            plus_res[r * col + c] = matrix1[r * col + c] + matrix2[r * col + c];
        }
    }
    return NULL;
}
void matrix_plus_pth(){
        pthread_t* thread_handles = (pthread_t*)malloc(thread_count * sizeof(pthread_t*));
        for(int tid=0; tid<thread_count; tid++){
            pthread_create(&thread_handles[tid], NULL, __matrix_plus_pth_divider, (long int)tid);
        }
        for(int tid=0; tid<thread_count; tid++){
            pthread_join(thread_handles[tid], NULL);
        }
        free(thread_handles);
}
void* __matrix_minus_pth_divider(void* tid){
    int each = row / thread_count;
    int start = (long)tid * each;
    int end = (long)tid == thread_count-1? row-1: ((long)tid + 1) * each -1;
    for(int r=start; r<=end; r++){
        for(int c=0; c<col; c++){
            minus_res[r * col + c] = matrix1[r * col + c] - matrix2[r * col + c];
        }
    }
    return NULL;
}
void matrix_minus_pth(){
        pthread_t* thread_handles = (pthread_t*)malloc(thread_count * sizeof(pthread_t*));
        for(int tid=0; tid<thread_count; tid++){
            pthread_create(&thread_handles[tid], NULL, __matrix_minus_pth_divider, (long int)tid);
        }
        for(int tid=0; tid<thread_count; tid++){
            pthread_join(thread_handles[tid], NULL);
        }
        free(thread_handles);
}
void* __matrix_multiply_pth_divider(void* tid){
    int each = row / thread_count;
    int start = (long)tid * each;
    int end = (long)tid == thread_count-1? row-1: ((long)tid + 1) * each -1;
    for(int r=start; r<= end; r++){
        for(int c=0; c< col; c++){
            multiply_res[r * col + c] = 0;
            for(int i=0; i< col; i++){
                multiply_res[r * col + c] += matrix1[r * col + i] * matrix2[i * col + c];
            }
        }
    }
    return NULL;
}
void matrix_multiply_pth(){
        pthread_t* thread_handles = (pthread_t*)malloc(thread_count * sizeof(pthread_t*));
        for(int tid=0; tid<thread_count; tid++){
            pthread_create(&thread_handles[tid], NULL, __matrix_multiply_pth_divider, tid);
        }
        for(int tid=0; tid<thread_count; tid++){
            pthread_join(thread_handles[tid], NULL);
        }
        free(thread_handles);
}
