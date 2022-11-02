#include <stdio.h>
#include <immintrin.h>
#include <time.h>
#include <windows.h>

#define N 100

struct A{
    int a;
    float b;
    double c;
    char d;
};

union B{
    int a;
    float b;
    double c;
    char d;
};


int main(){
    struct A a[N];
    union B b[N];
    int res_normal[N];
    int res[N];

    // init
    for(int i=0; i<N; i++){
        a[i].a = i;
        b[i].a = i;
    }

    //normal
    for(int i=0; i<N; i++)
        res_normal[i] = a[i].a + b[i].a;

    // simd
    for(int i=0; i<N; i+=4){
        __mmask8 add_mask = _cvtu32_mask8(0b01010101);
        __mmask8 store_mask = _cvtu32_mask8(0b01010101);
        __m256i zero_v = _mm256_setzero_si256();
        __m256i va1 = _mm256_loadu_si256((const __m256i_u*)(a+i));
        __m256i va2 = _mm256_loadu_si256((const __m256i_u*)(a+i+2));
        __m256i vb = _mm256_loadu_si256((const __m256i_u*)(b+i));
        int* tmp_a1 = (int*)&va1;
        int* tmp_a2 = (int*)&va2;
        __m256i va = _mm256_set_epi32(0, tmp_a2[6], 0, tmp_a2[0], 0, tmp_a1[6], 0, tmp_a1[0]);
        __m256i result = _mm256_mask_add_epi32(zero_v, add_mask, va, vb);
        _mm256_mask_compressstoreu_epi32(res+i, store_mask, result);
    }


    // check
    int err = 0;
    for(int i=0; i<N; i++)
        if(res[i] != res_normal[i]){
            err = 1;
            printf("%d\n", res[i]);
            break;
        }

    if(err)
        printf("err\n");
    else
        printf("right\n");


    return 0;
}

