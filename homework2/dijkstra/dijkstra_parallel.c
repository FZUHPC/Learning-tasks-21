#include <stdio.h>
#define size 9

const int thread_count = 8;
int graph[size][size] = {{0, 4, 0, 0, 0, 0, 0, 8, 0},
                {4, 0, 8, 0, 0, 0, 0, 11, 0},
                {0, 8, 0, 7, 0, 4, 0, 0, 2},
                {0, 0, 7, 0, 9, 14, 0, 0, 0},
                {0, 0, 0, 9, 0, 10, 0, 0, 0},
                {0, 0, 4, 14, 10, 0, 2, 0, 0},
                {0, 0, 0, 0, 0, 2, 0, 1, 6},
                {8, 11, 0, 0, 0, 0, 1, 0, 7},
                {0, 0, 2, 0, 0, 0, 6, 7, 0}};
int dis[size];
int trash_node[size] = {0};
typedef struct node{
    int num;
    int dis;
}node;

// queue
node q[10000] = {0};
int head_index = 0;
int tail_index = 0;
int ele_cnt = 0;
void queue_push(node val);
node queue_pop();
int queue_is_empty();
int queue_is_full();

void print_result();
void dj();


int main(){
    for(int i=0; i<size; i++){
        dis[i] = 0b01111111111111111111111111111111;
    }
    int start = 0;
    dj(start);
    print_result();
    return 0;
}

// queue
void queue_push(node n){
    q[tail_index++] = n;
    ele_cnt++;
}
node queue_pop(){
    node n = q[head_index++];
    ele_cnt--;
    return n;
}
int queue_is_empty(){
    return ele_cnt == 0;
}
int queue_is_full(){
    return ele_cnt == 100;
}

// other
void print_result(){
    for(int i=0; i<size; i++){
        printf("%d ", dis[i]);
    }
    printf("\n");
}
void dj(){
    queue_push((node){0, 0});
    dis[0] = 0;
    while(!queue_is_empty()){
        node cur = queue_pop();
        #pragma omp parallel for num_threads(thread_count)
        for(int i=0; i<size; i++){
            int d = graph[cur.num][i];
            if(d == 0){
                continue;
            }
            if(!trash_node[i] && dis[i] > d + cur.dis){
                dis[i] = d + cur.dis;
                queue_push((node){i, d + cur.dis});
            }
        }
        trash_node[cur.num] = 1;
    }
}