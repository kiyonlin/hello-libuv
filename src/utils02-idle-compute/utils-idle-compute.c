//
// Created by kiyon on 24/02/2018.
//

#include "utils-idle-compute.h"
#include <stdio.h>
#include <uv.h>

uv_idle_t idler_compute;
char buffer_idler_compute[1024];
uv_fs_t stdin_watcher_idler_compute;

void crunch_away(uv_idle_t *handle) {
    // Compute extra-terrestrial life
    // fold proteins
    // computer another digit of PI
    // or similar
    fprintf(stderr, "Computing PI...\n");
    // just to avoid overwhelming your terminal emulator
    uv_idle_stop(handle);
}

void on_type_idler_compute(uv_fs_t *req) {
    if (stdin_watcher_idler_compute.result > 0) {
        buffer_idler_compute[stdin_watcher_idler_compute.result] = '\0';
        printf("Typed %s\n", buffer_idler_compute);

        uv_buf_t buf = uv_buf_init(buffer_idler_compute, 1024);
        uv_fs_read(uv_default_loop(), &stdin_watcher_idler_compute, 0, &buf, 1, -1, on_type_idler_compute);
        uv_idle_start(&idler_compute, crunch_away);
    } else if (stdin_watcher_idler_compute.result < 0) {
        fprintf(stderr, "error opening file: %s\n", uv_strerror(req->result));
    }
}

void run_utils_idle_compute() {
    uv_idle_init(uv_default_loop(), &idler_compute);

    uv_buf_t buf = uv_buf_init(buffer_idler_compute, 1024);

    uv_fs_read(uv_default_loop(), &stdin_watcher_idler_compute, 0, &buf, 1, -1, on_type_idler_compute);

    uv_idle_start(&idler_compute, crunch_away);

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}