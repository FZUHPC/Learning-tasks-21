#pragma once

#include <stdint.h>

#define THREADS_NR 16

typedef struct thread_t {
    int tid;
    enum {
        RUNNING,
        SLEEPING,
        READY,
    } state;
    int64_t rsp;
    int64_t stack[1024];  // 4kb stack
} thread_t;

extern thread_t* threads_list[THREADS_NR];

// 创建线程，如果成功则返回0
// TODO： 实现带参数、返回值
int thread_create(thread_t* thread, void* (*routine)(void*));

// 将线程状态修改为sleeping并调度
int thread_park();

// 将线程从sleeping中唤醒
int thread_unpark(int tid);

int thread_join(int tid, void** ret);