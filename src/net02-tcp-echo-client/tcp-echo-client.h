//
// Created by kiyon on 12/02/2018.
//

#ifndef HELLO_LIBUV_TCP_ECHO_CLIENT_H
#define HELLO_LIBUV_TCP_ECHO_CLIENT_H

#include <uv.h>

void on_connect_tcp_client(uv_connect_t *req, int status);
void on_tcp_client_write_end(uv_write_t *req, int status);
void alloc_buffer_tcp_client(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
void echo_tcp_client_read(uv_stream_t *server, ssize_t nread, const uv_buf_t *buf);

void run_tcp_echo_client();
#endif //HELLO_LIBUV_TCP_ECHO_CLIENT_H
