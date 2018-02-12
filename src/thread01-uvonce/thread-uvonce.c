//
// Created by kiyon on 12/02/2018.
//

#include "thread-uvonce.h"

#include <stdio.h>
#include <uv.h>

static uv_once_t once = UV_ONCE_INIT;

static void init_once(void)
{
    printf("init once\n");
}

void worker(void *arg)
{
    uv_once(&once, init_once);
    printf("thread id:%lu\n", (unsigned long)uv_thread_self());
}

void run_thread_uvonce()
{
    uv_thread_t nthread1, nthread2;
    uv_thread_create(&nthread1, worker, NULL);
    uv_thread_create(&nthread2, worker, NULL);
    uv_thread_join(&nthread1);
    uv_thread_join(&nthread2);
}