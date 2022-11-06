#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;
const int N = 510;

int g[N][N];
int dist[N];
bool st[N];
int n, m;

int dijkstra()
{
    memset(dist, 0x3f, sizeof dist);
    
    dist[1] = 0;
    
    for(int i = 0; i < n; i ++)
    {
        int t = -1;
        for(int j = 1; j <= n; j ++)
            if(!st[j] && (t == -1 || dist[t] > dist[j]))
                t = j;
                
        for(int j = 1; j <= n; j ++)
            dist[j] = min(dist[j], dist[t] + g[t][j]);
            
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
    
    //cin >> n >> m;
    //cout << n << " " <<m << endl;
   
    memset(g, 0x3f, sizeof g);
    
    while(m --)
    {
        int a, b, c;
        
        fscanf(f1,"%d",&a);
        fscanf(f1,"%d",&b);
        fscanf(f1,"%d",&c); 
               
    //  cin >> a >> b >> c;
    //  cout << a << " " << b << " " << c << endl;
    
        g[a][b] = min(g[a][b], c);
    } 
   
   cout << dijkstra() << endl;
    
    fclose(f1);
}