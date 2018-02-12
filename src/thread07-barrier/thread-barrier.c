//
// Created by kiyon on 12/02/2018.
//

#include "thread-barrier.h"
#include <stdio.h>
#include <unistd.h>
#include <uv.h>

/**
 * 应用场景：等待一组进程全部执行完毕后再执行某些事
 * 屏障作为同步点，在所有有屏障的地方都会阻塞等待
 * 直到所有的线程都的代码都执行到同步点，再继续执行后续代码
 **/

uv_barrier_t barrier;

void worker1_barrier(void *arg)
{
    sleep(2);
    printf("worker1 done, thread id:%lu\n", (unsigned long)uv_thread_self());
    uv_barrier_wait(&barrier);
}

void worker2_barrier(void *arg)
{
    sleep(1);
    printf("worker2 done, thread id:%lu\n", (unsigned long)uv_thread_self());
    uv_barrier_wait(&barrier);
}

void callback_barrier(void *arg)
{
    printf("waiting for workers\n");
    uv_barrier_wait(&barrier);
}

void run_thread_barrier()
{
    uv_barrier_init(&barrier, 4);

    uv_thread_t thread1, thread2, thread3;
    uv_thread_create(&thread1, worker1_barrier, NULL);
    uv_thread_create(&thread2, worker2_barrier, NULL);
    uv_thread_create(&thread3, callback_barrier, NULL);

    // join等待线程结束，所以barrier不能放在jion后面，会导致死锁
    uv_barrier_wait(&barrier);

    uv_thread_join(&thread1);
    uv_thread_join(&thread2);
    uv_thread_join(&thread3);

    printf("all thread done\n");
    uv_barrier_destroy(&barrier);
}