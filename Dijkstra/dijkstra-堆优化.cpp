#include <iostream>
#include <cstring>
#include <queue>
#include <omp.h>
using namespace std;
const int N = 1e7 + 10;

typedef pair<int, int> PII;
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
    priority_queue<PII, vector<PII>, greater<PII>> heap;
    dist[1] = 0;
    heap.push({0, 1});
    
    while(heap.size())
    {
        auto t = heap.top();
        heap.pop();
        
        int x = t.second, distance = t.first;
        
        if(st[x]) continue;
        st[x] = true;
        
        for(int i = h[x]; i != -1; i = ne[i])
        {
            int j = e[i];
            if(dist[j] > dist[x] + w[i])
            {
                dist[j] = dist[x] + w[i];
                heap.push({dist[j], j});
            }
        }
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
        
        //cout << a << " " << b << " " << c << endl;
        add(a, b, c);
    }
    
    double t0,t1;
    double T0;
    
    t0 = omp_get_wtime();
    
    cout << dijsktra() << endl;
    
    t1 = omp_get_wtime();
    
    T0 = (t1-t0)*1000;
    printf("Dijkstra运行耗时: %f ms\n", T0);
    
    fclose(f1);
}