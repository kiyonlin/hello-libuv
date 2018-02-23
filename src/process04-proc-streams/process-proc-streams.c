//
// Created by kiyon on 23/02/2018.
//

#include "process-proc-streams.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include <uv.h>

uv_process_t child_req_stream;
uv_process_options_t options_stream;

void on_exit_stream(uv_process_t *, int64_t, int);

void run_process_streams() {
    size_t size = 500;
    char path[size];
    uv_exepath(path, &size);
    strcpy(path + (strlen(path) - strlen("hello_libuv")), "../src/process04-proc-streams/test");

    char *args[2];
    args[0] = path;
    args[1] = NULL;

    options_stream.stdio_count = 3;
    uv_stdio_container_t child_stdio[3];

    // 0 1 2 对应 stdin stdout stderr
    // UV_IGNORE重定向到/dev/null
    child_stdio[0].flags = UV_IGNORE;
    child_stdio[1].flags = UV_IGNORE;
    child_stdio[2].flags = UV_INHERIT_FD;
    child_stdio[2].data.fd = 2;

    options_stream.stdio = child_stdio;

    options_stream.exit_cb = on_exit_stream;
    options_stream.file = args[0];
    options_stream.args = args;

    int r;
    if ((r = uv_spawn(uv_default_loop(), &child_req_stream, &options_stream))) {
        fprintf(stderr, "%s\n", uv_strerror(r));
        return;
    }

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

void on_exit_stream(uv_process_t *req, int64_t exit_status, int term_signal) {
    fprintf(stderr, "Process exited with status %" PRId64 ", signal %d\n", exit_status, term_signal);
    uv_close((uv_handle_t *) req, NULL);
}
