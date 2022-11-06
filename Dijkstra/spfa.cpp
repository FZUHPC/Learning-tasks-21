#include <iostream>
#include <cstring>
#include <queue>
#include <algorithm>
#include <omp.h>
using namespace std;
const int N = 1e7 + 10;

int n, m;
int dist[N];
int h[N], e[N], ne[N], w[N], idx;
bool st[N];

void add(int a,int b, int c)
{
    e[idx] = b, w[idx] = c, ne[idx] = h[a], h[a] = idx ++;
}

int spfa()
{
    memset(dist, 0x3f, sizeof dist);
    dist[1] = 0;

    queue<int> q;
    q.push(1);
    st[1] = true;

    while (q.size())
    {
        int t = q.front();
        q.pop();

        st[t] = false;

        for (int i = h[t]; i != -1; i = ne[i])
        {
            int j = e[i];
            if (dist[j] > dist[t] + w[i])
            {
                dist[j] = dist[t] + w[i];
                if (!st[j])
                {
                    q.push(j);
                    st[j] = true;
                }
            }
        }
    }

    return dist[n];
}
int main()
{
    FILE * f1;		
    f1 = fopen("input.txt","r");
    fscanf(f1,"%d ",&n);	
    fscanf(f1,"%d",&m);

    memset(h, -1, sizeof h);

    while (m -- )
    {
        int a, b, c;
        fscanf(f1,"%d",&a);
        fscanf(f1,"%d",&b);
        fscanf(f1,"%d",&c); 
        add(a, b, c);
    }

    double t0,t1;
    double T0;
    
    t0 = omp_get_wtime();

    int t = spfa();

    t1 = omp_get_wtime();
    
    T0 = (t1-t0)*1000;
	printf("spfa运行耗时: %f ms\n", T0);

    if (t == 0x3f3f3f3f) puts("impossible");
    else printf("%d\n", t);
	
	fclose(f1);
    return 0;
}