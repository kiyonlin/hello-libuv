//
// Created by kiyon on 12/02/2018.
//

#include "thread-tls.h"
/**
 * 线程本地存储TLS是一个很有用的特性
 * TLS允许我们存储共享变量的副本
 * 这个副本只在这个线程里存在
 * 每个线程里都互相不影响
 **/

#include <stdio.h>
#include <uv.h>

uv_key_t key;
uv_barrier_t blocker;
int num08 = 0;

void worker_tls(void *arg)
{
    num08++;
    uv_key_set(&key, &num08);
    int my_num = *((int *)uv_key_get(&key));
    uv_barrier_wait(&blocker);
    printf("num is %d, my num is %d\n", num08, my_num);
}

void run_thread_tls()
{
    uv_key_create(&key);

    uv_barrier_init(&blocker, 3);

    uv_thread_t thread1, thread2, thread3;

    uv_thread_create(&thread1, worker_tls, NULL);
    uv_thread_create(&thread2, worker_tls, NULL);
    uv_thread_create(&thread3, worker_tls, NULL);

    uv_thread_join(&thread1);
    uv_thread_join(&thread2);
    uv_thread_join(&thread3);

    printf("done\n");
    uv_key_delete(&key);
}