//
// Created by kiyon on 24/02/2018.
//

#include "thread-progress.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <uv.h>

uv_async_t async_progress;
double percentage;

void fake_download(uv_work_t *req) {
    int size = *(int *) req->data;
    int downloaded = 0;
    while (downloaded <= size) {
        percentage = downloaded * 100.0 / size;
        async_progress.data = (void *) &percentage;
        uv_async_send(&async_progress);

        sleep(1);
        downloaded += (500 + random()) % 1000;
    }
}

void after_progress(uv_work_t *req, int status) {
    fprintf(stderr, "Download complete\n");
    uv_close((uv_handle_t *) &async_progress, NULL);
}

void print_progress(uv_async_t *handle) {
    double percentage = *(double *) handle->data;
    fprintf(stderr, "Download %.2f%%\n", percentage);
}

void run_thread_progress() {
    uv_work_t req;
    int size = 10240;
    req.data = (void *) &size;

    uv_async_init(uv_default_loop(), &async_progress, print_progress);
    uv_queue_work(uv_default_loop(), &req, fake_download, after_progress);

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}