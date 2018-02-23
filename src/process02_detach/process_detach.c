//
// Created by kiyon on 23/02/2018.
//

#include "process_detach.h"
#include <stdio.h>
#include <uv.h>

uv_process_t child_req_detach;

uv_process_options_t options_detach;

void run_process_detach() {
    char *args[3];
    args[0] = "sleep";
    args[1] = "60";
    args[2] = NULL;

    options_detach.exit_cb = NULL;
    options_detach.file = "sleep";
    options_detach.args = args;
    options_detach.flags = UV_PROCESS_DETACHED;

    int r;
    if ((r = uv_spawn(uv_default_loop(), &child_req_detach, &options_detach))) {
        fprintf(stderr, "%s\n", uv_strerror(r));
        return;
    }
    fprintf(stderr, "Launched sleep with ID %d\n", child_req_detach.pid);

    uv_unref((uv_handle_t *) &child_req_detach);

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}