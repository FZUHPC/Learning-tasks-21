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
