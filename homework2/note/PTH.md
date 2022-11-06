# pthread

ps: 想运行后面代码编译指令为`gcc file.c -o file -lpthread`，需要添加`-lpthread`选项

[toc]

## 前置知识

### 线程

操作系统进行运算调度的最小单位，被包含在进程之中，是进程中的实际运作单位。

一条线程指的是进程中一个单一顺序的控制流，一个进程中可以并发多个线程，每个线程并行执行不同的任务。

### 多线程

多个控制线程 或 多个控制流

多线程的内存空间是共享的，通信比较简单

### 线程标准

pthread是POSIX线程标准的实现，是一种**并行执行**的模型，也是一个**共享内存**的模型

### 线程的状态信息

- 就绪：线程等待可用的处理器
- 运行：线程正在被执行
- 阻塞：线程正在等待某个时间的发生
- 终止：线程从开始函数中返回或者调用pthread_exit

![image-20221106030254802](https://cgy-1310478750.cos.ap-nanjing.myqcloud.com//202211060303181.png)



### hello world

```c
int main(int argc, char* argv[]){
    long thread;
    pthread_t* thread_handles;
    thread_count = strtol(argv[1], NULL, 10);
    thread_handles = malloc(thread_count * sizeof(pthread_t));
    
    for(thread=0; thread<thread_count; thread++){
				// 第一个记得传指针
        pthread_create(&thread_handles[thread], NULL, Hello, (void*)thread);
    }
    printf("hello from the main thread\n");
    for(thread=0; thread<thread_count; thread++){
        pthread_join(thread_handles[thread], NULL);
    }
    free(thread_handles);
    return 0;
}

void* Hello(void* rank){
    long my_rank = (long)rank;
    printf("hello from thread %ld of %d\n", my_rank, thread_count);
    return NULL;
}
```

### 临界区

在多线程中，对一个全局变量进行修改的时候，会导致读取了先前的值，进行计算，而本应该去获取其他线程计算完的值，再计算

### 忙等待

通过设置全局变量如flag，进行线程的判断，进入一个循环，直到前面的线程计算结束，执行了`flag++`之后，才退出循环。貌似这个是一个很不可取的操作。

```c
flag++;
while(flag != tid){}
// some code
```

### 互斥锁

限制每次只能有一个线程进入临界区，在已经有线程进入临界区的情况下，其他线程不能进入

```c
// 声明
pthread_mutex_t mutex;
// 初始化
mutex = pthread_mutex_init(&mutex, NULL);
// 使用
pthread_mutex_lock(&mutex);
sum += my_sum;
pthread_mutex_unlock(&mutex);
```

注意到有一些操作其实是可以优化的，比较一下

不好的

```c
for(int i=start; i<end; i++){
		pthread_mutex_lock(&mutex);
		sum += my_sum;
		pthread_mutex_unlock(&mutex);
}
```

正确的

```c
int my_sum = 0;
for(int i=start; i<end; i++){
    my_sum += i;
}
pthread_mutex_lock(&mutex);
sum += my_sum;
pthread_mutex_unlock(&mutex);
```

上锁还是要消耗资源的，所以能少上就少上几个

> 互斥锁的实现：本身就是共享的资源，会有多个线程进行访问。为了避免这个作为共享资源的锁的线程安全问题，解决方法就是让操作这个共享资源的方式，是原子的。
>
> 实现原子操作的方式就是一条指令就可以操作这个共享资源。

### 读写锁

[参考链接](https://blog.csdn.net/lovecodeless/article/details/24968369)

共享资源的访问分为**读者**和**写者**，读者只对共享资源进行读访问，写者只对共享资源进行写操作。

如果是使用互斥锁，读、写都需要独占共享资源

如果是使用读写锁，允许多个读者访问共享资源，只有写者才要独占共享资源。相比互斥机制，读写机制由于允许多个读者同时读访问共享资源，进一步提高了多线程的并发度。

### 自旋锁

> 不放弃CPU的时间片，不断尝试获取锁。
>
> 个人理解，好像跟忙等待是一个东西？的样子

自旋锁不会使线程进入休眠，会持续占用CPU资源。

非自旋锁会让线程切换状态，进入休眠，CPU在这段时间就可以做其他事情，直到持有锁的线程释放了锁，此时CPU把线程恢复，让线程尝试获取锁。成功获取锁就执行后面代码，失败就再次进入休眠。

**优劣**

看似自旋锁会一直占用CPU，但是非自旋锁的阻塞和唤醒线程都是需要高昂的开销的。同步代码块中的内容不复杂的话，转换线程带来的开销比实际业务代码执行的更大。

**使用场景**

并发度不太高，临界区小

### 路障

实现1：互斥量+忙等待

缺陷：忙等待是会一直占用cpu，消耗资源

- 具体实现
  
    ```c
    #include <pthread.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    
    const int thread_count = 5;
    int counter = 0;
    pthread_mutex_t mutex;
    
    void* Hello(void* arg){
        printf("thread %ld start\n", (long)arg);
    
        // 给计数器上锁
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    
        // 挂起线程3
        if((long)arg == 3){
            printf("wating for thread 3");
            sleep(2);
        }
    
        // 忙等待
        while(counter < thread_count){}
        
        // 同步后运行的代码
        printf("thread %ld end, counter = %d\n", (long)arg, counter);
        return NULL;
    }
    
    int main(){
        pthread_t* thread_handles;
        pthread_mutex_init(&mutex, NULL);
        thread_handles = malloc(thread_count * sizeof(pthread_t));
        for(int tid=0; tid<thread_count; tid++){
            pthread_create(&thread_handles[tid], NULL, Hello, (void*)tid);
        }
        for(int tid=0; tid<thread_count; tid++){
            pthread_join(thread_handles[tid], NULL);
        }
        pthread_mutex_destroy(&mutex);
    }
    ```
    

实现2：信号量

- 基础api：
  
    `sem_t` 、`sem_init` 、`sem_post` 、`sem_wait` 、`sem_destroy`
    
    sem_post和sem_wait都是原子操作，sem_post对信号量+1，sem_wait对信号量-1。其中当信号量非零的时候，才会-1，否则进行阻塞。如果对一个值为0的信号量调用sem_wait，这个函数就会地等待直到有其它线程增加了这个值使它不再是0为止。
    

缺陷，barrier_sem这个路障重复利用的时候，可能导致路障失效

- eg:
  
    ps: `@1` `@2` `@3`在底下的注释里
    
    假设有两个线程，当线程0阻塞在`@1`处时，线程1运行了`@3`，此时`barrier_sem`的信号量为2，然后线程1运行了`@1`，此时`barrier_sem`的信号量为1。然而线程0有可能还是被挂起的状态，没来得及运行`@1`，这时候线程1已经进入到了下一个路障，运行了`@2`，此时`barrier_sem`的信号量减到了0。结果就是只有线程1运行了所有代码，线程0停留在了第一个路障。
    
    ```c
    void* Hello(void* arg){
    		// 路障1		
    		sem_wait(&count_sem);
    		if(counter == thread_count-1){
    		    counter = 0;
    		    sem_post(&count_sem);
    		    for(int i=0; i<thread_count; i++){
    		        sem_post(&barrier_sem);  // @3
    		    }
    		}else{
    		    counter++;
    		    sem_post(&count_sem);
    		    sem_wait(&barrier_sem); // @1
    		}
    		//some code
    		
    		// 路障2
    		sem_wait(&count_sem);
    		if(counter == thread_count-1){
    		    counter = 0;
    		    sem_post(&count_sem);
    		    for(int i=0; i<thread_count; i++){
    		        sem_post(&barrier_sem);
    		    }
    		}else{
    		    counter++;
    		    sem_post(&count_sem);
    		    sem_wait(&barrier_sem); // @2
    		}
    		
    		// some code
    		return NULL;
    }
    ```
    
- 具体实现：
  
    ```c
    #include <pthread.h>
    #include <semaphore.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <stdlib.h>
    
    const int thread_count = 5;
    int counter = 0;
    sem_t count_sem, barrier_sem;
    
    void* Hello(void* arg){
        printf("tid = %d\n", (long)arg);
    
        // 进行阻塞, 将count_sem的信号量减为0, 保护counter, 类似于上锁操作吧
        sem_wait(&count_sem);
    
        // 挂起线程4
        if((long)arg == 4){
            sleep(2);
        }
        if(counter == thread_count-1){
    				// 让counter为0, 重新初始化计数器, 后面还可以用count设新的路障
            counter = 0;
            // 对应起上面的sem_wait(&count_sem);
            // 类似于开锁的
            sem_post(&count_sem);
            for(int i=0; i<thread_count; i++){
                // 将信号量加到thread_count
                sem_post(&barrier_sem);
            }
        }else{
            counter++;
            // 对应起上面的sem_wait(&count_sem);
            // 类似于开锁的操作
            sem_post(&count_sem);
            // 在运行最后一个线程前, 这里会一直阻塞着
    
            /*
                在最后一个线程做完thread_count次sem_post(&barrier_sem);之后
                barrier_sem的信号量就变成thread_count
                此时barrier_sem的信号量就可以开始减-1
            */
            sem_wait(&barrier_sem);
        }
    		// 同步后运行的代码
        printf("thread %d, counter=%d\n", (long)arg, counter);
    		return NULL;
    }
    
    int main(){
        sem_init(&count_sem, 0, 1);  // 将count_sem的值初始化为1, 作用是保护counter
        sem_init(&barrier_sem, 0, 0);  // 将barrier_sem的值初始化为0, 路障
        pthread_t* thread_handles;
        thread_handles = malloc(thread_count * sizeof(pthread_t));
        for(int tid=0; tid<thread_count; tid++){
            pthread_create(&thread_handles[tid], NULL, Hello, (void*)tid);
        }
        for(int tid=0; tid<thread_count; tid++){
            pthread_join(thread_handles[tid], NULL);
        }
        sem_destroy(&count_sem);
        sem_destroy(&barrier_sem);
        return 0;
    }
    ```
    

实现3：条件变量

条件变量总是和一个互斥锁绑定

- 具体实现
  
    ```c
    #include <pthread.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <stdlib.h>
    
    const int thread_count = 5;
    int counter = 0;
    pthread_mutex_t mutex;
    pthread_cond_t cond_var;
    
    void* Hello(void* arg){
        if((long)arg == 2){
            sleep(2);
        }
        pthread_mutex_lock(&mutex);
        counter++;
        if(counter == thread_count){
            counter = 0;
            pthread_cond_broadcast(&cond_var);
        }else{
            while(pthread_cond_wait(&cond_var, &mutex) != 0){}
        }
        pthread_mutex_unlock(&mutex);
        printf("tid: %ld counter = %d\n", (long)arg, counter);
    }
    
    int main(){
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&cond_var, NULL);
    
        pthread_t* thread_handles = (pthread_t*)malloc(thread_count * sizeof(pthread_t));
        for(int tid=0; tid<thread_count; tid++){
            pthread_create(&thread_handles[tid], NULL, Hello, tid);
        }
        for(int tid=0; tid<thread_count; tid++){
            pthread_join(thread_handles[tid], NULL);
        }
    
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond_var);
    }
    ```
### 关于传参
在学习的过程中会看到很多时候，传参只需要传递线程号，像这样`pthread_create(&thread_handles[tid], NULL, Hello, tid);`，然后在函数中通过`long tid = (long)arg`来获取。这是因为传参的类型是`void*`，而`sizeof(void*) == sizeof(long) == 8`，所以可以这样转换。

那么如果传递多个参数呢？就可以通过结构体指针来传参。

## 常用API

### 创建

#### pthread_once

> 线程一次性初始化，即异步执行一个函数一次

第一个参数：是一个控制变量吧，``pthread_once_t *__once_control``，注意传递的是指针，使用``PTHREAD_ONCE_INIT``这个宏

第二个参数：传入函数指针，这个函数只执行一次

```c
#include <stdio.h>
#include <pthread.h>

pthread_once_t once = PTHREAD_ONCE_INIT;  

void once_func(void){
    printf("this is once func\n");
}

void* func(void* tid){
    pthread_once(&once, once_func);
    printf("%ld\n", (long)tid);
}

int main(){
    pthread_t threads[5];
    for(long i=0; i<5; i++){
        pthread_create(&threads[i], NULL, func, (void*)i);
    }

    for(long i=0; i<5; i++){
        pthread_join(threads[i], NULL);
    }
    return 0;
}

```

#### pthread_create

> 创建线程

第一个参数为`pthread_t*`

第二个为线程属性

第三个是函数指针

第四个是传入的参数，类型是`void*`

### 终止

#### pthread_join

> 线程合并，主线程**阻塞**等待子线程执行完毕以后，回收资源，继续执行主线程内容

#### pthread_detach

> 线程分离，将子线程从主线程分离，由系统回收资源

#### pthread_cancel

> 发送一个停止线程的信号，并不意味着线程终止

#### pthread_exit

>终止当前子线程

跟return的区别：pthread_exit可以自动调用线程清理程序，释放所持有的特定数据绑定(特定数据为pthread_key_t)

### 线程的标识

#### pthread_self

> 获取当前线程号

#### pthread_equal

> 比较传入的两个线程是否相等

### 特定数据

[参考代码](https://www.cnblogs.com/zhangxuan/p/6515264.html)

> 同一个线程内，不同的函数中，需要通过这样的特定数据来共享访问数据

#### int pthread_key_create(pthread_key_t* key, void(*destructor)(void *))

> 特定数据的创建函数

主线程中必须要使用pthread_exit(0)，否则，主线程就没有执行销毁函数，导致没有释放空间。子线程中，在调用pthread_join时，就会调用销毁函数了

第一个参数为特定变量的指针

第二个为特定变量的销毁函数，释放空间等等

#### void* pthread_getspecific(pthread_key_t key)

> 返回当前线程特定数据的首地址

#### int pthread_setspecific(pthread_key_t key, const void* value)

> 设定特定数据的值

给线程特定数据申请空间之后，把申请空间的首地址传入即可

#### int pthread_key_delete

> 删除特定数据，删除所有跟这个key关联到的内存

### 线程调度策略

[参考链接](https://www.cnblogs.com/cynchanpin/p/6897070.html)

#### pthread_getschedparam

#### pthread_setschedparam

| 调度策略    | 解释                                                         |
| ----------- | ------------------------------------------------------------ |
| SCHED_FIFO  | 先入先出策略，让一个任务完成后，再执行另一个任务。仅适合优先级大于0的线程 |
| SCHED_RR    | 给每个进程一个固定的时间片，时间片用完，还没执行完就暂停，让其他线程运行。轮转依次运行 |

### 安全

#### pthread_atfork

## 线程属性

还没尝试过，先蛮记一下

| 属性        | 默认值                  | 含义                |
| ----------- | ----------------------- | ------------------- |
| scope       | PTHREAD_SCOPE           | 线程竞争范围？？    |
| detachstate | PTHREAD_CREATE_JOINABLE | 分离状态？？        |
| stackaddr   | NULL                    | 栈地址              |
| stacksize   | 0                       | 系统默认栈大小      |
| priority    | 0                       | 线程优先级，默认为0 |
| schedpolicy | SCHED_OTHER             | 调度策略            |
| guardsize   | PAGESIZE                | 栈保护区大小        |

### 创建销毁

#### pthread_attr_init(pthread_attr_t* attr)

#### pthread_attr_destroy(pthread_attr_t* attr)

