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
