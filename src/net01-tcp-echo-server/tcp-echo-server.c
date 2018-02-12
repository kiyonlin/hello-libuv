//
// Created by kiyon on 12/02/2018.
//

#include "tcp-echo-server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#define DEFAULT_PORT 7000
#define DEFAULT_BACKLOG 128

uv_loop_t *loop_tcp_server;

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

void free_write_req(uv_write_t *req) {
    write_req_t *wr = (write_req_t *) req;
    free(wr->buf.base);
    free(wr);
}

void alloc_buffer_tcp_server(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = (char *) malloc(suggested_size);
    buf->len = suggested_size;
}

void echo_tcp_server_write(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    free_write_req(req);
}

void echo_tcp_server_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread > 0) {
        write_req_t *req = (write_req_t *) malloc(sizeof(write_req_t));
        req->buf = uv_buf_init(buf->base, (unsigned int) nread);
        printf("server received: %s\n", buf->base);
        uv_write((uv_write_t *) req, client, &req->buf, 1, echo_tcp_server_write);
        return;
    }
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name((int) nread));
        uv_close((uv_handle_t *) client, NULL);
    }

    free(buf->base);
}

void on_connect_tcp_server(uv_stream_t *server, int status) {
    if (status < 0) {
        fprintf(stderr, "New tcp connection error %s\n", uv_strerror(status));
        // error!
        return;
    }

    uv_tcp_t *client = (uv_tcp_t *) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop_tcp_server, client);
    // 4.uv_accept接收链接。
    if (uv_accept(server, (uv_stream_t *) client) == 0) {
        // 5.使用stream处理来和客户端通信。
        uv_read_start((uv_stream_t *) client, alloc_buffer_tcp_server, echo_tcp_server_read);
    } else {
        uv_close((uv_handle_t *) client, NULL);
    }
}

void run_tcp_echo_server() {
    loop_tcp_server = uv_default_loop();

    // 1.uv_tcp_init建立tcp句柄。
    uv_tcp_t server;
    uv_tcp_init(loop_tcp_server, &server);

    struct sockaddr_in addr;
    uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);

    // 2.uv_tcp_bind绑定。
    uv_tcp_bind(&server, (const struct sockaddr *) &addr, 0);

    // 3.uv_listen建立监听，当有新的连接到来时，激活调用回调函数。
    // 第二个参数是待处理的连接请求队列－最大长度的请求连接队列。
    int r = uv_listen((uv_stream_t *) &server, DEFAULT_BACKLOG, on_connect_tcp_server);

    if (r) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
        return;
    }

    uv_run(loop_tcp_server, UV_RUN_DEFAULT);
}