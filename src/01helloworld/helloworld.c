//
// Created by kiyon on 11/02/2018.
//

#include "helloworld.h"
#include <uv.h>
#include <stdlib.h>

void run_hello_world() {
    uv_loop_t *loop = malloc(sizeof(uv_loop_t));
    uv_loop_init(loop);

    printf("Hello world.\n");
    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
    free(loop);
}
