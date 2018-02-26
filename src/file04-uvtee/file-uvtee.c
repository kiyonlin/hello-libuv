//
// Created by kiyon on 26/02/2018.
//

#include "file-uvtee.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <uv.h>

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

uv_pipe_t stdin_pipe_uvtee;
uv_pipe_t stdout_pipe_uvtee;
uv_pipe_t file_pipe_uvtee;

void alloc_buffer_uvtee(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}

void free_write_req_uvtee(uv_write_t *req) {
    write_req_t *wr = (write_req_t *) req;
    free(wr->buf.base);
    free(wr);
}

void on_stdout_write_uvtee(uv_write_t *req, int status) {
    free_write_req_uvtee(req);
}

void on_file_write_uvtee(uv_write_t *req, int status) {
    free_write_req_uvtee(req);
}

void write_data_uvtee(uv_stream_t *dest, size_t size, uv_buf_t buf, uv_write_cb cb) {
    write_req_t *req = (write_req_t *) malloc(sizeof(write_req_t));
    req->buf = uv_buf_init((char *) malloc(size), (unsigned int) size);
    memcpy(req->buf.base, buf.base, size);
    uv_write((uv_write_t *) req, dest, &req->buf, 1, cb);
}

void read_stdin_uvtee(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {
    if (nread < 0) {
        if (nread == UV_EOF) {
            uv_close((uv_handle_t *) &stdin_pipe_uvtee, NULL);
            uv_close((uv_handle_t *) &stdout_pipe_uvtee, NULL);
            uv_close((uv_handle_t *) &file_pipe_uvtee, NULL);
        }
    } else if (nread > 0) {
        write_data_uvtee((uv_stream_t *) &stdout_pipe_uvtee, (size_t) nread, *buf, on_stdout_write_uvtee);
        write_data_uvtee((uv_stream_t *) &file_pipe_uvtee, (size_t) nread, *buf, on_file_write_uvtee);
    }
}

void run_file_uvtee(int argc, char **argv) {
    uv_pipe_init(uv_default_loop(), &stdin_pipe_uvtee, 0);
    uv_pipe_open(&stdin_pipe_uvtee, 0);

    uv_pipe_init(uv_default_loop(), &stdout_pipe_uvtee, 0);
    uv_pipe_open(&stdout_pipe_uvtee, 1);

    uv_fs_t file_req;
    int fd = uv_fs_open(uv_default_loop(), &file_req, argv[1], O_CREAT, 0644, NULL);
    printf("fd:%d\n", fd);
    uv_pipe_init(uv_default_loop(), &file_pipe_uvtee, 0);
    uv_pipe_open(&file_pipe_uvtee, fd);

    uv_read_start((uv_stream_t *) &stdin_pipe_uvtee, alloc_buffer_uvtee, read_stdin_uvtee);

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}