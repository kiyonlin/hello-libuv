//
// Created by kiyon on 24/02/2018.
//

#include "utils-ref-timer.h"
#include <stdio.h>
#include <uv.h>

void gc_ref_timer(uv_timer_t *handle) {
    fprintf(stderr, "Freeing unused objects\n");
}

void fake_job_ref_timer(uv_timer_t *handle){
    fprintf(stdout, "Fake job done\n");
}

void run_utils_ref_timer(){
    uv_timer_t gc_req, fake_job_req;
    uv_timer_init(uv_default_loop(), &gc_req);
    uv_timer_init(uv_default_loop(), &fake_job_req);

    uv_unref((uv_handle_t*) &gc_req);

    uv_timer_start(&gc_req, gc_ref_timer, 0, 2000);
    uv_timer_start(&fake_job_req, fake_job_ref_timer, 7000, 0);

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}