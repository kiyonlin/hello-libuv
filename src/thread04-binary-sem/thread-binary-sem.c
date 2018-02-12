//
// Created by kiyon on 12/02/2018.
//

#include "thread-binary-sem.h"
#include <stdio.h>
#include <unistd.h>
#include <uv.h>

/**
 * 信号量有两个操作，分别是P操作和V操作，uv_sem_wait对应P操作，uv_sem_post对应V操作。
 * 信号量用一个数值S表示，在uv_sem_init的时候可以设置。
 * 当执行P操作时，判断S是否大于0，如果不大于0，那么无法进入临界区，代码会阻塞，
 * 如果大于0，那么S -= 1，进入临界区。当执行V操作时，S+=1，如果S大于0，那么就会唤醒其他进程被阻塞的代码进入临界区。
 *
 * 信号量的两个操作P和V都是原子性的，不可能被打断，所以可以保证S的变化是可控的。
 **/

uv_sem_t sem;

void worker_binary_sem(void *arg)
{
    uv_sem_wait(&sem);
    sleep(2);

    printf("thread id:%lu\n", (unsigned long)uv_thread_self());
    uv_sem_post(&sem);
}

void run_thread_binary_sem()
{
    uv_sem_init(&sem, 1);

    uv_thread_t thread1, thread2;
    uv_thread_create(&thread1, worker_binary_sem, NULL);
    uv_thread_create(&thread2, worker_binary_sem, NULL);

    uv_thread_join(&thread1);
    uv_thread_join(&thread2);
}