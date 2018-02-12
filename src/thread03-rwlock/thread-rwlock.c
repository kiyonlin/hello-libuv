//
// Created by kiyon on 12/02/2018.
//

#include "thread-rwlock.h"
#include <stdio.h>
#include <unistd.h>
#include <uv.h>

uv_rwlock_t lock;
int num03 = 0;

void writer_rwlock(void *arg)
{
    uv_rwlock_wrlock(&lock);
    sleep(2);
    num03++;
    printf("writer, num is %d\n", num03);
    uv_rwlock_wrunlock(&lock);
}

void reader_rwlock(void *arg)
{
    uv_rwlock_rdlock(&lock);
    sleep(2);
    printf("reader, num is %d\n", num03);
    uv_rwlock_rdunlock(&lock);
}

void run_thread_rwlock()
{
    uv_rwlock_init(&lock);

    const int N = 6;
    uv_thread_t threads[N];

    uv_thread_create(&threads[2], reader_rwlock, NULL);
    uv_thread_create(&threads[4], reader_rwlock, NULL);
    uv_thread_create(&threads[0], writer_rwlock, NULL);
    uv_thread_create(&threads[5], reader_rwlock, NULL);
    uv_thread_create(&threads[3], reader_rwlock, NULL);
    uv_thread_create(&threads[1], writer_rwlock, NULL);

    for (int i = 0; i < N; i++)
    {
        uv_thread_join(&threads[i]);
    }
}