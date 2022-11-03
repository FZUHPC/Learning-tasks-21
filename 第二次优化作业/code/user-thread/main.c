#include <pthread.h>
#include <scheduler.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <time.h>
#include <timer.h>
#include <user-thread.h>

thread_t* current_thread;
thread_t* threads_list[THREADS_NR] = {0};

thread_t main_thread;

int times = 0;

// 添加主线程到thread_list中
void init_thread_list() {
    current_thread = &main_thread;
    threads_list[0] = &main_thread;
    main_thread.tid = 0;
    main_thread.state = RUNNING;
}

void timer_func() {
    times++;
    printf("Schedule...%d\n", times);
    schedule();
}

void* a(void* args) {
    char i = 0;
    while (1) {
        printf("%d\n", i);
        if (i++ == 0)
            schedule();
    }
}

void* b(void* args) {
    int a = 0;
    while (1) {
        printf("b\n");
        if (a++ == 0)
            schedule();
    }
}

void* c(void* args) {
    while (1) {
        // printf("c\n");
    }
}

int main() {
    signal(SIGVTALRM, &schedule);
    //如果只留一个主线程，timer_func又会执行了
    init_thread_list();
    start_timer();
    thread_t ta;
    thread_t tb;
    thread_t tc;
    thread_create(&ta, a);
    thread_create(&tb, b);
    thread_create(&tc, c);


    while (1) {
        printf("main...\n");
    }
    return 0;
}
