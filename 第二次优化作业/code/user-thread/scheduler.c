#include <scheduler.h>

extern thread_t* current_thread;
extern thread_t* threads_list[THREADS_NR];

extern void context_switch(int64_t* from_sp, int64_t* to_sp);

// 先简单模拟一个调度器
// TODO： 实现MLFQ调度
thread_t* find_next_thread() {
    thread_t* ret = current_thread;
    for (uint8_t i = current_thread->tid + 1; i != current_thread->tid; i %= THREADS_NR) {
        if (threads_list[i] != 0 && threads_list[i]->state == READY) {
            ret = threads_list[i];
            break;
        }
        i++;
    }
    return ret;
}

void schedule() {
    thread_t* next = find_next_thread();
    if (current_thread->state == RUNNING)
        current_thread->state = READY;
    next->state = RUNNING;
    thread_t* old = current_thread;
    current_thread = next;
    context_switch(&(old->rsp), &(next->rsp));
    return;
}