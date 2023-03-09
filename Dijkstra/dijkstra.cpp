#include <iostream>
#include <string.h>
#include <omp.h>
using namespace std;

const int N = 1e7 + 10;

#define likely(x) __builtin_expect(!!(x), 1) 
#define unlikely(x) __builtin_expect(!!(x), 0)

int n, m;
int h[N], e[N], w[N], ne[N], idx;
int dist[N];
bool st[N];

void add(int a,int b, int c)
{
    e[idx] = b, w[idx] = c, ne[idx] = h[a], h[a] = idx ++;
}

int dijsktra()
{
    memset(dist, 0x3f, sizeof dist);
    dist[1] = 0;
    
    for(int i=0;i<n-1;i++){
        int t=-1;

        
#pragma omp parallel for 
        for(int j=1;j<=n;j++){
        register int *k = dist;
            if(!st[j] && (t==-1 || *(k+t) > *(k+j)))
                t = j;
        }
        

        for(int j=h[t]; j!=-1; j=ne[j]){
            int k = e[j];
            if(unlikely(dist[k] > dist[t]+w[j])){
                dist[k] = dist[t]+w[j];
            }
        }
        st[t] = true;
    }
    
    
    if(dist[n] == 0x3f3f3f3f) return -1;
    return dist[n];
}

int main()
{

    FILE * f1;		
    f1 = fopen("input.txt","r");
    fscanf(f1,"%d ",&n);	
    fscanf(f1,"%d",&m);
    
   // cin >> n >> m;
    memset(h, -1, sizeof h);
    
    while(m --)
    {
        int a, b, c;
        //cin >> a >> b >> c;
        
        fscanf(f1,"%d",&a);
        fscanf(f1,"%d",&b);
        fscanf(f1,"%d",&c); 
        
     //   cout << a << " " << b << " " << c << endl;
        add(a, b, c);
    }
    
    double t0,t1;
    double T0;
    
    t0 = omp_get_wtime();
    
    printf("ANSWER is %d\n",dijsktra());
    
    t1 = omp_get_wtime();
    
    T0 = (t1-t0)*1000;
    printf("Dijkstra运行耗时: %f ms\n", T0);
    
    fclose(f1);
}