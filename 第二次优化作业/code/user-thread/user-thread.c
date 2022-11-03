#include <scheduler.h>
#include <user-thread.h>

extern thread_t* current_thread;
extern thread_t* threads_list[THREADS_NR];

extern void context_init(int64_t* rsp);

int thread_create(thread_t* thread, void* (*routine)(void*)) {
    // 为新线程找slot, 并将下标作为线程的tid
    unsigned int i;
    for (i = 0; i < THREADS_NR; i++) {
        if (threads_list[i] == 0) {
            threads_list[i] = thread;
            break;
        }
    }
    if (i == THREADS_NR)
        return -1;
    // 初始化线程的信息
    thread->tid = i;
    thread->state = READY;
    thread->stack[1023] = (int64_t)routine;
    thread->rsp = (int64_t)&(thread->stack[1022]);
    context_init(&thread->rsp);
    return 0;
}

// TODO： 测试
int thread_park() {
    current_thread->state = SLEEPING;
    schedule();
    return 0;
}

// TODO： 测试
int thread_unpark(int tid) {
    threads_list[tid]->state = READY;
    return 0;
}

// TODO: 实现
int thread_join(int tid, void** ret) {
    return 0;
}
