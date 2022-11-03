#include <pthread.h>
#include <condition_variable>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <iterator>
#include <mutex>
#include <queue>
#include <thread>

class threadpool_t {
    // 考虑线程池需要对外提供的接口
   public:
    // constructor
    threadpool_t(std::size_t n = 4) {
        threads_list = new std::thread*[n];
        for (size_t i = 0; i < n; i++)
            threads_list[i] = new std::thread(thread_routine, this);
        this->thread_num = n;
    }

    // 添加任务
    void add_task(void* (*task)(void*), void* args) {
        {
            std::unique_lock<std::mutex> lck(this->mutex);
            task_queue.push((task_t){task, args});
            has_task.notify_one();
        }
    }

    // 返回当前任务是否已经完成
    bool is_finished() {
        bool ret;
        ret = this->task_queue.empty();
        return ret;
    }

    // destructor
    ~threadpool_t() {
        for (size_t i = 0; i < thread_num; i++) {
            delete threads_list[i];
        }
        delete[] threads_list;
    }

   private:
    struct task_t {
        void* (*task)(void*);
        void* args;
    };

    // 辅助函数
    static void thread_routine(threadpool_t* self) {
        while (true) {
            task_t* task;
            {
                //这里有并发的一个经典坑
                //即该线程被唤醒后并不意味着马上得到调度执行，
                //如果另一个消费者先得到调度并将任务取走执行，
                //等到调度回该线程时，就会出现无资源可用的问题，因此要用while
                //(或是使用第二个参数)
                std::unique_lock<std::mutex> lck(self->mutex);
                self->has_task.wait(lck, [self] { return !self->task_queue.empty(); });
                //取出任务，释放锁
                task = &self->task_queue.front();
                self->task_queue.pop();
            }
            //执行任务
            task->task(task->args);
        }
    }

    // 考虑线程池需要的数据
    size_t thread_num;
    std::thread** threads_list;
    std::queue<task_t> task_queue;
    std::mutex mutex;
    std::condition_variable has_task;
};