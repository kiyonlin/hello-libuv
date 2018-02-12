//
// Created by kiyon on 12/02/2018.
//

#include "dns.h"
#include <uv.h>
#include <stdlib.h>

uv_loop_t *loop_dns;

void alloc_buffer_dns(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}

void on_read_dns(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name((int) nread));

        uv_close((uv_handle_t *) client, NULL);
        free(buf->base);
        free(client);
        return;
    }

    char *data = (char *) malloc(sizeof(char) * (nread + 1));
    data[nread] = '\0';
    strncpy(data, buf->base, nread);

    fprintf(stderr, "data:%s", data);
    free(data);
    free(buf->base);
}

void on_connect_dns(uv_connect_t *req, int status) {
    if (status < 0) {
        fprintf(stderr, "connect failed error %s\n", uv_err_name(status));
        return;
    }

    uv_read_start((uv_stream_t *) req->handle, alloc_buffer_dns, on_read_dns);

    free(req);
}

void on_resolved(uv_getaddrinfo_t *resolver, int status, struct addrinfo *res) {
    if (status < 0) {
        fprintf(stderr, "getaddrinfo callback error %s\n", uv_err_name(status));
        return;
    }

    char addr[17] = {0};
    uv_ip4_name((struct sockaddr_in *) res->ai_addr, addr, 16);
    fprintf(stderr, "ip:%s\n", addr);

    uv_connect_t *connect_req = (uv_connect_t *) malloc(sizeof(uv_connect_t));
    uv_tcp_t *socket = (uv_tcp_t *) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop_dns, socket);

    uv_tcp_connect(connect_req, socket, (const struct sockaddr *) res->ai_addr, on_connect_dns);

    uv_freeaddrinfo(res);
}

void run_dns() {
    loop_dns = uv_default_loop();

    struct addrinfo hints;
    hints.ai_family = PF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = 0;

    uv_getaddrinfo_t resolver;
    fprintf(stderr, "irc.freenode.net is... \n");
    int r = uv_getaddrinfo(loop_dns, &resolver, on_resolved, "irc.freenode.net", "6667", &hints);

    if (r) {
        fprintf(stderr, "getaddrinfo call error %s\n", uv_err_name(r));
        return;
    }

    uv_run(loop_dns, UV_RUN_DEFAULT);
}