//
// Created by kiyon on 11/02/2018.
//

#ifndef HELLO_LIBUV_IDLE_BASIC_H
#define HELLO_LIBUV_IDLE_BASIC_H

#include <uv.h>

void wait_for_a_while(uv_idle_t *handle);

void run_idle_basic();

#endif //HELLO_LIBUV_IDLE_BASIC_H
