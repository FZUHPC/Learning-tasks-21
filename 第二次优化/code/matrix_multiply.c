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
