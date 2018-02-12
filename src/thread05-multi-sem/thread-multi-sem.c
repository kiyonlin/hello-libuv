//
// Created by kiyon on 12/02/2018.
//

#include "thread-multi-sem.h"
#include <stdio.h>
#include <unistd.h>
#include <uv.h>

uv_sem_t sem;

int num05 = 0;

void reader_multi_sem(void *arg)
{
    uv_sem_wait(&sem);
    uv_sem_post(&sem);

    sleep(2);
    printf("reader, num is %d\n", num05);
}

void writer_multi_sem(void *arg)
{
    printf("writer, num is %d\n", num05);
    num05++;
    uv_sem_post(&sem);
}

void run_thread_multi_sem()
{
    uv_sem_init(&sem, 0);
    uv_thread_t thread1, thread2, thread3;

    uv_thread_create(&thread1, reader_multi_sem, NULL);
    uv_thread_create(&thread2, reader_multi_sem, NULL);
    uv_thread_create(&thread3, writer_multi_sem, NULL);

    uv_thread_join(&thread1);
    uv_thread_join(&thread2);
    uv_thread_join(&thread3);
}