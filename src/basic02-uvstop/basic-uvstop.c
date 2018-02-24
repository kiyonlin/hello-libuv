//
// Created by kiyon on 24/02/2018.
//

#include "basic-uvstop.h"
#include <stdio.h>
#include <uv.h>

int64_t counter_stop = 0;

void idle_stop_cb(uv_idle_t *handle) {
    printf("Idle callback, type: %d\n", handle->type);

    if(++counter_stop > 5) {
        uv_stop(handle->loop);
//        uv_stop(uv_default_loop());
        printf("uv_stop() called\n");
    }
}

void prep_stop_cb(uv_prepare_t *handle) {
    printf("Prep callback, type: %d\n", handle->type);
}

void run_uv_stop() {
    uv_idle_t idler;
    uv_prepare_t prep;

    uv_idle_init(uv_default_loop(), &idler);
    uv_idle_start(&idler, idle_stop_cb);

    uv_prepare_init(uv_default_loop(), &prep);
    uv_prepare_start(&prep, prep_stop_cb);

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}