//
// Created by kiyon on 23/02/2018.
//

#include "process-spawn.h"
#include <stdio.h>
#include <inttypes.h>
#include <uv.h>

uv_process_t child_req_spawn;

// 由于现在的options是全局变量，因此被初始化为0。
// 如果在局部变量中定义options，要将所有没用的域设为0
// uv_process_options_t options = {0};
uv_process_options_t options_spawn;

void on_exit_spawn(uv_process_t *req, int64_t exit_status, int term_signal) {
    fprintf(stderr, "Process exited with status %" PRId64 ", signal %d\n", exit_status, term_signal);
    uv_close((uv_handle_t *) req, NULL);
}

void run_process_spawn() {
    char *args[3];
    args[0] = "mkdir";
    args[1] = "test-dir";
    args[2] = NULL;

    options_spawn.exit_cb = on_exit_spawn;
    // file是要执行的程序
    options_spawn.file = "mkdir";
    options_spawn.args = args;

    int r;
    if ((r = uv_spawn(uv_default_loop(), &child_req_spawn, &options_spawn))) {
        fprintf(stderr, "%s\n", uv_strerror(r));
        return;
    } else {
        fprintf(stderr, "Launched process with ID %d\n", child_req_spawn.pid);
    }

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

}