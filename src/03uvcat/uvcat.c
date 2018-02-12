//
// Created by kiyon on 11/02/2018.
//

#include "uvcat.h"
#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <uv.h>

uv_fs_t open_req;
uv_fs_t read_req;
uv_fs_t write_req;

static char buffer_uvcat[1024];

static uv_buf_t iov_uvcat;

void on_write_uvcat(uv_fs_t *req) {
    if (req->result < 0) {
        fprintf(stderr, "Write error: %s\n", uv_strerror((int)req->result));
    }
    else {
        uv_fs_read(uv_default_loop(), &read_req, open_req.result, &iov_uvcat, 1, -1, on_read_uvcat);
    }
}

void on_read_uvcat(uv_fs_t *req) {
    if(req->result <0) {
        fprintf(stderr, "Read error: %s\n", uv_strerror((int)req->result));
    } else if (req->result ==0 ) {
        uv_fs_t close_req;
        // synchronous
        uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
    } else if(req->result >0 ) {
        iov_uvcat.len = req->result;
        uv_fs_write(uv_default_loop(), &write_req, 1, &iov_uvcat, 1, -1, on_write_uvcat);
    }
}

void on_open_uvcat(uv_fs_t *req) {
    // The request passed to the callback is the same as the one the call setup
    // function was passed.
    assert(req == &open_req);
    if(req->result >= 0) {
        iov_uvcat = uv_buf_init(buffer_uvcat, sizeof(buffer_uvcat));
        uv_fs_read(uv_default_loop(), &read_req, req->result, &iov_uvcat, 1, -1, on_read_uvcat);
    } else {
        fprintf(stderr, "error opening file: %s\n", uv_strerror((int)req->result));
    }
}

void run_uvcat(int argc, char** argv){
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file1>\n", argv[0]);
        return ;
    }

    uv_fs_open(uv_default_loop(), &open_req, argv[1], O_RDONLY, 0, on_open_uvcat);

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    uv_fs_req_cleanup(&open_req);
    uv_fs_req_cleanup(&read_req);
    uv_fs_req_cleanup(&write_req);
}
