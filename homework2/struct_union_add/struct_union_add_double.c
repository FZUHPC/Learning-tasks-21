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
    double res_normal[N];
    double res[N];

    // init
    for(int i=0; i<N; i++){
        a[i].c = i + 1.0 * i / 10;
        b[i].c = i + 1.0 * i / 10;
    }

    //normal
    for(int i=0; i<N; i++)
        res_normal[i] = a[i].c + b[i].c;

    // simd
    for(int i=0; i<100; i+=4) {
        __m256d va1 = _mm256_loadu_pd(&a[i].c);
        __m256d va2 = _mm256_loadu_pd(&a[i + 2].c);
        __m256d va = _mm256_set_pd(va2[3], va2[0], va1[3], va1[0]);
        __m256d vb = _mm256_loadu_pd((double*)(b+i));
        __m256d result = _mm256_add_pd(va, vb);
        _mm256_storeu_pd(res+i, result);
    }

    // check
    int err = 0;
    for(int i=0; i<N; i++)
        if(res[i] != res_normal[i]){
            err = 1;
            printf("%lf\n", res[i]);
            break;
        }

    if(err)
        printf("err\n");
    else
        printf("right\n");


    return 0;
}

