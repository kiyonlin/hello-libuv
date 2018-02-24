//
// Created by kiyon on 24/02/2018.
//

#include "thread-queue-work.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <uv.h>

#ifndef FIB_UNTIL
#define FIB_UNTIL 25
#endif

long fib_qw_(long t) {
    if (t == 0 || t == 1)
        return 1;
    else
        return fib_qw_(t - 1) + fib_qw_(t - 2);
}

void fib_qw(uv_work_t *req) {
    int n = *(int *) req->data;
    if (random() % 2)
        sleep(1);
    else
        sleep(3);

    long fib = fib_qw_(n);
    fprintf(stderr, "%dth fibonacci is %lu\n", n, fib);
}

void after_fib_qw(uv_work_t *req, int status) {
    fprintf(stderr, "Done calculating %dth fibonacci\n", *(int *) req->data);
}

void run_thread_queue_work() {
    int data[FIB_UNTIL];
    uv_work_t req[FIB_UNTIL];
    for (int i = 0; i < FIB_UNTIL; ++i) {
        data[i] = i;
        req[i].data = (void *) &data[i];
        uv_queue_work(uv_default_loop(), &req[i], fib_qw, after_fib_qw);
    }

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}