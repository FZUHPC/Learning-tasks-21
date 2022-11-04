/*
    ref:https://zhuanlan.zhihu.com/p/98652938

    因为是平转的原因上周才开始上线代..导致这个求矩阵的逆看了好几天还是没有很明白,后面实在写不了矩阵除法

    不过大意是明白的:求矩阵相除,实际上就是把除数求一个逆矩阵,然后再相乘
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

#define MAXN 500

using namespace std;

FILE *f1,*f2,*f3;
int method;

double a[MAXN][MAXN],b[MAXN][MAXN],c[MAXN][MAXN];

void init()
{
    // 221030 更新了一下学长给的数据!
    f1 = fopen("data_a.txt","r");
    f2 = fopen("data_b.txt","r");
    f3 = fopen("out.txt","w");

    puts("INPUT METHOD: 0:ADD 1:SUB 2:MUL 3:DIV");
    scanf("%d",&method);

    if(method < 0 || method > 3)
    {
        puts("ERROR METHOD!");
        exit(0);
    }

    if(method == 3)
    {
        puts("NOT SUPPORT TEMPORARY!");
        exit(0);
    }
}

void close()
{
    fclose(f1);
    fclose(f2);
    fclose(f3);
}

void solve()
{
    // READ
    for(int i=1;i<MAXN;i++)
    {
        for(int j=1;j<MAXN;j++)
        {
            fscanf(f1,"%lf",&a[i][j]);
            fscanf(f2,"%lf",&b[i][j]);
        }
    }

    // SOLVE
    clock_t start,finish;
    double totaltime;
    start = clock();

    for(int i=1;i<MAXN;i++)
    {
        for(int j=1;j<MAXN;j++)
        {
            if(method == 0) c[i][j] = a[i][j] + b[i][j]; // ADD
            if(method == 1) c[i][j] = a[i][j] - b[i][j]; // SUB
            if(method > 1)
            {
                for(int k=1;k<MAXN;k++)
                {
                    c[i][j] += a[i][k] * b[k][j]; // MUL/DIV
                }
            }
        }
    }

    // OUT
    for(int i=1;i<MAXN;i++)
    {
        for(int j=1;j<MAXN;j++)
        {
            fprintf(f3,"%lf ",c[i][j]);
        }
    }

    puts("SOLVE SUCCESS -> out.txt!");

    finish = clock();
    totaltime = (double)(finish-start)/CLOCKS_PER_SEC*1000;
    printf("TIME CONSUME: %.6lf ms\n",totaltime);
}

int main()
{
    init();
    solve();
    close();
    return 0;
}