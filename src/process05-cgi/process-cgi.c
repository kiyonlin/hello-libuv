//
// Created by kiyon on 23/02/2018.
//

#include "process-cgi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <uv.h>

uv_process_t child_req_cgi;
uv_process_options_t options_cgi;

void cleanup_handles_cgi(uv_process_t *, int64_t, int);

void invoke_cgi_script(uv_tcp_t *);

void on_new_connection_cgi(uv_stream_t *, int);

void run_process_cgi() {
    uv_tcp_t server;
    uv_tcp_init(uv_default_loop(), &server);

    struct sockaddr_in bind_addr;
    uv_ip4_addr("0.0.0.0", 7000, &bind_addr);
    uv_tcp_bind(&server, (const struct sockaddr *) &bind_addr, 0);

    int r = uv_listen((uv_stream_t *) &server, 128, on_new_connection_cgi);

    if (r) {
        fprintf(stderr, "Listen error %s\n", uv_err_name(r));
        return;
    }

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

void cleanup_handles_cgi(uv_process_t *req, int64_t exit_status, int term_signal) {
    fprintf(stderr, "Process exited with status %" PRId64 ", signal %d\n", exit_status, term_signal);
    uv_close((uv_handle_t *) req->data, NULL);
    uv_close((uv_handle_t *) req, NULL);
}

void invoke_cgi_script(uv_tcp_t *client) {
    size_t size = 500;
    char path[size];
    uv_exepath(path, &size);
    strcpy(path + (strlen(path) - strlen("hello_libuv")), "../src/process05-cgi/tick");

    char *args[2];
    args[0] = path;
    args[1] = NULL;

    options_cgi.stdio_count = 3;
    uv_stdio_container_t child_stdio[3];
    child_stdio[0].flags = UV_IGNORE;
    child_stdio[1].flags = UV_INHERIT_STREAM;
    child_stdio[1].data.stream = (uv_stream_t *) client;
    child_stdio[2].flags = UV_IGNORE;

    options_cgi.stdio = child_stdio;
    options_cgi.exit_cb = cleanup_handles_cgi;
    options_cgi.file = args[0];
    options_cgi.args = args;

    // Set this so we can close the socket after the chile process exits.
    child_req_cgi.data = (void *) client;

    int r;
    if ((r = uv_spawn(uv_default_loop(), &child_req_cgi, &options_cgi))) {
        fprintf(stderr, "%s\n", uv_strerror(r));
        return;
    }
}

void on_new_connection_cgi(uv_stream_t *server, int status) {
    if (status == -1) {
        fprintf(stderr, "Cgi tcp connection error %s\n", uv_strerror(status));
        return;
    }

    uv_tcp_t *client = (uv_tcp_t *) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(uv_default_loop(), client);

    if (uv_accept(server, (uv_stream_t *) client) == 0) {
        invoke_cgi_script(client);
    } else {
        uv_close((uv_handle_t *) client, NULL);
    }
}