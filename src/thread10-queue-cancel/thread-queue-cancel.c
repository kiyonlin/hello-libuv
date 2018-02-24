//
// Created by kiyon on 24/02/2018.
//

#include "thread-queue-cancel.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <uv.h>

#ifndef FIB_UNTIL
#define FIB_UNTIL 25
#endif

uv_work_t fib_reqs_qc[FIB_UNTIL];

long fib_qc_(long t) {
    if (t == 0 || t == 1)
        return 1;
    else
        return fib_qc_(t - 1) + fib_qc_(t - 2);
}

void fib_qc(uv_work_t *req) {
    int n = *(int *) req->data;
    if (random() % 2)
        sleep(1);
    else
        sleep(3);

    long fib = fib_qc_(n);
    fprintf(stderr, "%dth fibonacci is %lu\n", n, fib);
}

void after_fib_qc(uv_work_t *req, int status) {
    fprintf(stderr, "Done calculating %dth fibonacci\n", *(int *) req->data);
}

void signal_handler_qc(uv_signal_t *req, int signum) {
    printf("Signal received!\n");
    for (int i = 0, r; i < FIB_UNTIL; ++i) {
        if ((r = uv_cancel((uv_req_t *) &fib_reqs_qc[i])) < 0)
            fprintf(stderr, "Cancel fib[%d] failed: %s\n", i, uv_strerror(r));
        else
            fprintf(stdout, "Cancel fib[%d] successful\n", i);
    }

    uv_signal_stop(req);
}

void run_thread_queue_cancel() {
    int data[FIB_UNTIL];
    for (int i = 0; i < FIB_UNTIL; ++i) {
        data[i] = i;
        fib_reqs_qc[i].data = (void *) &data[i];
        uv_queue_work(uv_default_loop(), &fib_reqs_qc[i], fib_qc, after_fib_qc);
    }

    uv_signal_t sig;
    uv_signal_init(uv_default_loop(), &sig);
    uv_signal_start(&sig, signal_handler_qc, SIGINT);

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}