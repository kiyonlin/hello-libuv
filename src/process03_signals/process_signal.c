//
// Created by kiyon on 23/02/2018.
//

#include "process_signal.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <uv.h>

void signal_handler(uv_signal_t *, int);

void thread1_worker_signal(void *);

void thread2_worker_signal(void *);

void run_process_signal() {
    printf("PID %d\n", getpid());

    uv_thread_t thread1, thread2;

    uv_thread_create(&thread1, thread1_worker_signal, 0);
    uv_thread_create(&thread2, thread2_worker_signal, 0);

    uv_thread_join(&thread1);
    uv_thread_join(&thread2);
}

void signal_handler(uv_signal_t *handle, int signum) {
    printf("Signal received: %d\n", signum);
    uv_signal_stop(handle);
}

// two signal handlers in one loop
void thread1_worker_signal(void *userp) {
    uv_loop_t *loop1 = uv_loop_new();

    uv_signal_t sig1a, sig1b;
    uv_signal_init(loop1, &sig1a);
    uv_signal_start(&sig1a, signal_handler, SIGUSR1);

    uv_signal_init(loop1, &sig1b);
    uv_signal_start(&sig1b, signal_handler, SIGUSR1);

    uv_run(loop1, UV_RUN_DEFAULT);
}

// two signal handlers, each in its own loop
void thread2_worker_signal(void *userp) {
    uv_loop_t *loop2 = uv_loop_new();
    uv_signal_t sig2;
    uv_signal_init(loop2, &sig2);
    uv_signal_start(&sig2, signal_handler, SIGUSR1);

    uv_loop_t *loop3 = uv_loop_new();
    uv_signal_t sig3;
    uv_signal_init(loop3, &sig3);
    uv_signal_start(&sig3, signal_handler, SIGUSR1);

    // uv_run(loop, UV_RUN_NOWAIT)和uv_run(loop, UV_RUN_ONCE)非常像，因为它们都只处理一个事件。
    // 不同在于，UV_RUN_ONCE会在没有任务的时候阻塞，但是UV_RUN_NOWAIT会立刻返回。
    // 使用NOWAIT，这样才使得一个loop不会因为另外一个loop没有要处理的事件而挨饿。
    while (uv_run(loop2, UV_RUN_NOWAIT) || uv_run(loop3, UV_RUN_NOWAIT)) {

    }
}
