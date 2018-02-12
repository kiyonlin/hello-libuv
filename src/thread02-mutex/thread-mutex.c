//
// Created by kiyon on 12/02/2018.
//

#include "thread-mutex.h"

#include <stdio.h>
#include <unistd.h>
#include <uv.h>

static uv_mutex_t lock;

void workerMutex(void *arg)
{
    int *num = (int *)arg;
    // int tryLock = uv_mutex_trylock(&lock);
    // printf("try lock: %d", tryLock);

    // if (tryLock < 0)
    // {
    //     printf("Mutex locked!\n");
    //     return;
    // }
    uv_mutex_lock(&lock);
    sleep(2);
    printf("thread id:%lu, value:%d\n", (unsigned long)uv_thread_self(), *num);
    uv_mutex_unlock(&lock);
}

void run_thread_mutex()
{
    uv_mutex_init(&lock);

    uv_thread_t thread1, thread2;

    int a = 1, b = 2;

    uv_thread_create(&thread2, workerMutex, (void *) &b);
    uv_thread_create(&thread1, workerMutex, (void *) &a);

    uv_thread_join(&thread1);
    uv_thread_join(&thread2);
}