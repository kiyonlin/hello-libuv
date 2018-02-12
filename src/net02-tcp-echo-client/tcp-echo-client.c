//
// Created by kiyon on 12/02/2018.
//

#include "tcp-echo-client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

uv_loop_t *loop_tcp_client;

void echo_tcp_client_read(uv_stream_t *server, ssize_t nread, const uv_buf_t *buf) {
    if (nread == -1) {
        fprintf(stderr, "error echo_read");
        return;
    }

    printf("result: %s\n", buf->base);

    // 获取服务端数据后退出
    uv_close((uv_handle_t *) server, NULL);
}

void alloc_buffer_tcp_client(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void on_tcp_client_write_end(uv_write_t *req, int status) {
    if (status == -1) {
        fprintf(stderr, "error on_write_end");
        return;
    }

    uv_read_start(req->handle, alloc_buffer_tcp_client, echo_tcp_client_read);
}

void on_connect_tcp_client(uv_connect_t *req, int status) {
    if (status < 0) {
        fprintf(stderr, "New tcp connection error %s\n", uv_strerror(status));
        return;
    }

    char *message = "hello uv tcp";
    size_t len = strlen(message);

    char buffer[100];
    uv_buf_t buf = uv_buf_init(buffer, sizeof(buffer));

    buf.len = len;
    buf.base = message;

    uv_stream_t* tcp = req->handle;

    uv_write_t write_req;

    unsigned int buf_count = 1;

    uv_write(&write_req, tcp, &buf, buf_count, on_tcp_client_write_end);
}

void run_tcp_echo_client() {
    loop_tcp_client = uv_default_loop();

    uv_tcp_t client;

    uv_tcp_init(loop_tcp_client, &client);

    struct sockaddr_in dest;
    uv_ip4_addr("127.0.0.1", 7000, &dest);

    uv_connect_t connect_req;

    uv_tcp_connect(&connect_req, &client, (const struct sockaddr *)&dest, on_connect_tcp_client);

    uv_run(loop_tcp_client, UV_RUN_DEFAULT);
}