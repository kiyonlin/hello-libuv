//
// Created by kiyon on 11/02/2018.
//

#include <stdio.h>
#include <uv.h>

uv_idle_t idler;

int64_t counter_idle = 0;

void wait_for_a_while(uv_idle_t *handle) {
    counter_idle++;

    if (counter_idle >= 10e6) {
        uv_idle_stop(handle);
    }
}

void run_idle_basic() {

    uv_idle_init(uv_default_loop(), &idler);

    uv_idle_start(&idler, wait_for_a_while);

    printf("Idling...\n");
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    uv_loop_close(uv_default_loop());
}