#include <pthread.h>
#include <scheduler.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <time.h>
#include <timer.h>

pthread_t timer_thread;

void init_timer() {
    struct itimerval tTimer;
    memset((void*)&tTimer, 0, sizeof(tTimer));
    //定时周期1s
    tTimer.it_value.tv_sec = 0;
    tTimer.it_value.tv_usec = 1;
    tTimer.it_interval.tv_sec = 1;
    tTimer.it_interval.tv_usec = 0;
    setitimer(ITIMER_VIRTUAL, &tTimer, NULL);
}

void* timer_routine(void* args) {
    // 定时器线程屏蔽信号， 否则会导致定时器被调度...
    sigset_t mask;
    init_timer();
    sigaddset(&mask, SIGVTALRM);
    pthread_sigmask(SIG_SETMASK, &mask, NULL);
    while (1);
}

void start_timer() {
    // 启动定时器线程
    pthread_create(&timer_thread, NULL, timer_routine, NULL);
}