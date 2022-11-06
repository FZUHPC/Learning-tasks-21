# Dijkstra算法



## 基础版 dijkstra算法

基于邻接表存图

```c
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
        
/*  
 		for(int j=1;j<=n;j++){
            if(!st[j] && (t==-1 || dist[t] > dist[j]))
                t = j;
        }
*/
        
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
```



### Baseline

<img src="C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221104220655082.png" alt="image-20221104220655082" style="zoom:80%;" />



### 编译选项

![image-20221106000922944](C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221106000922944.png)

```shell
-O3 -ffast-math -funroll-all-loops -mavx -mtune=native -fopenmp 
```

<img src="C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221106001108007.png" alt="image-20221106001108007" style="zoom:80%;" />



### 热点分析

<img src="C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221105023037563.png" alt="image-20221105023037563" style="zoom:50%;" />

**Average Effective CPU Utilization: 0.999**

Target Utilization: 8

<img src="C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221104235636817.png" alt="image-20221104235636817" style="zoom:50%;" />

<img src="C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221104235811918.png" alt="image-20221104235811918" style="zoom:50%;" />



### 并行化



![image-20221106002930567](C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221106002930567.png)



<img src="C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221105011001667.png" alt="image-20221105011001667" style="zoom:80%;" />



<img src="C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221106001408741.png" alt="image-20221106001408741" style="zoom:50%;" />

Average Effective CPU Utilization: 6.648  （平均有效CPU利用率:6.648）

Target Utilization: 8  （目标利用率:8）

<img src="C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221106001445964.png" alt="image-20221106001445964" style="zoom:50%;" />

效果还阔以



## 堆优化版 dijkstra算法

```c
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
```



### Baseline

![image-20221106003229983](C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221106003229983.png)

### 编译选项

```shell
-O3 -ffast-math -funroll-all-loops -mavx -mtune=native -fopenmp 
```



![image-20221106003325212](C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221106003325212.png)

### VTune

<img src="C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221106004037881.png" alt="image-20221106004037881" style="zoom: 67%;" />



## spfa最短路 (队列)

```c
const int N = 1e7 + 10;

int n, m;
int dist[N];
int h[N], e[N], ne[N], w[N], idx;
bool st[N];

void add(int a,int b, int c){
    e[idx] = b, w[idx] = c, ne[idx] = h[a], h[a] = idx ++;
}

int spfa(){
    memset(dist, 0x3f, sizeof dist);
    dist[1] = 0;

    queue<int> q;
    q.push(1);
    st[1] = true;

    while (q.size()){
        int t = q.front();
        q.pop();
        st[t] = false;
        for (int i = h[t]; i != -1; i = ne[i]){
            int j = e[i];
            if (dist[j] > dist[t] + w[i]){
                dist[j] = dist[t] + w[i];
                if (!st[j]){
                    q.push(j);
                    st[j] = true;
                }
            }
        }
    }
    return dist[n];
}
```

### Baseline

<img src="C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221106011633868.png" alt="image-20221106011633868" style="zoom: 80%;" />

### 编译选项

```shell
-O3 -ffast-math -funroll-all-loops -mavx -mtune=native -fopenmp 
```

<img src="C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221106011903415.png" alt="image-20221106011903415" style="zoom: 80%;" />





## 并行化Dijkstra算法

**传统串行 Dijkstra 算法**

- 采用广度优先搜索思想，它的主要特点是选定起始点后，一个点一个点地求取最短距离，通过邻点逐步扩展，不断更新，直至求出起始点到目标点的最短距离后才停止。

- 假定赋权图G（V，E，W），V、E、W分别为图的顶点集、图的边集和图的边权集。假定起点为 u0，设集合 S ∈ V ，初始时集合 S 中仅包含起点 u0。对于任意一个属于顶点集 V-S 的顶点 u1，若已知起点 u0 至该顶点u1 的最短距离，则将该顶点u1放入集合S中，并记录起点u0到该顶点u1的最短距离。

- 在集合 V-S 中找到距离起点 u0 最近的顶点，记为顶点q，将顶点q加入集合S的同时将顶点q从集合V-S中移除。然后将顶点 q 视为新的起点，重复之前的步骤，直至目标点被加入集合 S 中。



**Dijkstra 最短路径算法本身存在并行化的不足**

- 集合 S （标记点集合） 每次循环迭代之后定点个数都会加 1，每次迭代都依赖于上次迭代的结果，循环之间存在依赖关系

**选择可并行化的主要区域**

![image-20221106145902575](C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221106145902575.png)

- 选择未标注节点中的最小节点：未标记节点以无序的形式存放在一个数组或一个链表内，每次选择最短路径节点都必须把所有未标记节点扫描一遍，当节点数目较大时，这将成为制约计算速度的关键因素。

### 并行算法（一）

每个线程计算一个顶点的所有边，从中取得最小边并保存在一个数组的不同位置，然后从数组中找出最小的值，得到最近距离的一个顶点

<img src="C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221106150115540.png" alt="image-20221106150115540" style="zoom:50%;" />
$$
md(u i ，v i ) = w(u i ，v j ) + Len(v j )
$$
利用多线程，直接对于**所有的集合S**的点进行求取md处理

跟基础的Dijkstra有点不一样

<img src="C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221106152648887.png" alt="image-20221106152648887" style="zoom:50%;" />



<img src="C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221106153642262.png" alt="image-20221106153642262" style="zoom: 50%;" />

### 并行算法（二）

两个线程的并行化

采用**双线程**分别从起点和终点同时按照 Dijkstra 算法最短路径的搜索分析，采用节点标记法动态地进行子网分割，最终将两个子网的搜索结果进行拼接汇总，获取最短路径。

思想：

<img src="C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221106154649960.png" alt="image-20221106154649960" style="zoom:50%;" />

最后根据拓扑关系进行路径集合的拼接，得出最短路径



<img src="C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221106155024040.png" alt="image-20221106155024040" style="zoom: 67%;" />

优点：

易于线程间的协调控制和数据通信，两个线程在处理过程中根据节点的状态标记，动态地进行两个子网的分割，一个线程处理慢了，另一个线程可以进行“帮忙”，直到处理完毕，两个子网才划分结束

软硬件的要求低

资源利用率高

缺点：

并行化程度不高

边界处理较为复杂



### 并行算法（三）

**定义：**

将 L 定义为起点 u0 到另一个顶点 u1 的**最短**路径权值，若顶点 u1 在第 r 步获得了标号 L，则称顶点 u1 在第
r 步获得永久性标号，r ≥ 0

将 l 定义为起点u0到顶点u1的最短路径权值**上限**。若顶点 u1 在第 r 步可以达到 l，则称 u1 在第 r 步获得了临时
性标号，r ≥ 0

**理论：**

在选择永久性标号的过程中，如果在第 r 步时，有多个顶点已经获得最小临时性标号，那么应该同时赋予这些顶点永久性标号

**目的：**

明显减少对临时性标号集合的遍历次数，达到缩短计算时间，提升计算效率的目的；同时也是并行化的理想对象

可以尽可能地减少永久性标号选择次数，进而缩短计算时间，提升算法运行效率

定理1 Dijkstra标号法中，在同一步将多个拥有最小标号 l 的顶点标记上标号 L，最短路的计算结果不变。

<img src="C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221106170403405.png" alt="image-20221106170403405" style="zoom: 33%;" />



并行多标号Dijkstra 算法的时间复杂度:
$$
O （ h （ n / k + logn ））
$$




核心采用**分治法**：将复杂问题等效转化为多个等价的较简单的子问题

<img src="C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221106161203098.png" alt="image-20221106161203098" style="zoom:50%;" />



> ps.     OpenMP耗时：
>
> critical 指令实现枷锁效果：使用锁来实现互斥：原子操作 =  7：3.5：1



由于需要修改获得永久性标号 L 顶点的邻点，导致多标号的Dijkstra需要修改的点数增加

储存临时标号 l 的数组作为此并行算法的**共享数据**

分配各个线程的工作量，让**线程固定处理若干个顶点**的标号 l 值，从而做到**数据独立**

![image-20221106164832202](C:\Users\Religious·J\AppData\Roaming\Typora\typora-user-images\image-20221106164832202.png)

<img src="file:///C:\Users\Religious·J\Documents\Tencent Files\1569978990\Image\Group2\BR\D9\BRD9_S55UQLXA5[KHW@CJVL.jpg" alt="img" style="zoom:33%;" />

