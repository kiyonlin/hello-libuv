//
// Created by kiyon on 11/02/2018.
//

#ifndef HELLO_LIBUV_UVCAT_H
#define HELLO_LIBUV_UVCAT_H

#include <uv.h>

void on_read(uv_fs_t *req);
void on_write(uv_fs_t *req);
void on_open(uv_fs_t *req);
void run_uvcat(int argc, char** argv);

#endif //HELLO_LIBUV_UVCAT_H
