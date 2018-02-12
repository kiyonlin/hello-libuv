//
// Created by kiyon on 12/02/2018.
//

#include "thread-cond.h"
#include <stdio.h>
#include <unistd.h>
#include <uv.h>

/**
 * 条件量的一大特点就是可以设置条件来控制uv_cond_signal的执行，
 * 进而我们可以利用它来阻塞代码执行，
 * 所以在线程池的实现中，
 * 条件变量是必不可少的，
 * 所有的工作塞进队列里，
 * 当队列为空的时候我们可以利用条件变量来阻塞代码。
 */
uv_mutex_t lock;
uv_cond_t cond;
int num06;

void worker1_cond(void *arg)
{
    while (1)
    {
        uv_mutex_lock(&lock);
        sleep(2);
        num06++;
        printf("waiting for num >0\n");
        // uv_cond_wait(&cond, &lock);
        uv_cond_timedwait(&cond, &lock, 3);
        printf("num is %d\n", num06);
        num06--;
        uv_mutex_unlock(&lock);
    }
}

void worker2_cond(void *arg)
{
    while (1)
    {
        uv_mutex_lock(&lock);
        sleep(2);
        if (num06 > 0)
        {
            printf("unlock cond\n");
            // uv_cond_signal(&cond);
            uv_cond_broadcast(&cond);
        }
        uv_mutex_unlock(&lock);
    }
}

void run_thread_cond()
{
    uv_mutex_init(&lock);
    uv_cond_init(&cond);

    uv_thread_t thread1, thread2;

    uv_thread_create(&thread1, worker1_cond, NULL);
    uv_thread_create(&thread2, worker2_cond, NULL);

    uv_thread_join(&thread1);
    uv_thread_join(&thread2);
}
