//
// Created by kiyon on 24/02/2018.
//

#include "file-onchange.h"
#include <stdio.h>
#include <stdlib.h>

#include <uv.h>

const char *command_onchange;

void run_command_onchange(uv_fs_event_t *handle, const char *filename, int events, int status) {
    char path[1024];
    size_t size = 1023;

    uv_fs_event_getpath(handle, path, &size);
    path[size] = '\0';

    fprintf(stderr, "Change detected in %s: ", path);
    if (events & UV_RENAME)
        fprintf(stderr, "renamed");
    if (events & UV_CHANGE)
        fprintf(stderr, "changed");

    fprintf(stderr, ". event:%d, file name:%s\n", events, filename ? filename : "");
    system(command_onchange);
}

void run_file_onchange(int argc, char **argv) {
    if (argc <= 2) {
        fprintf(stderr, "Usage: %s <command> <file1> [file2 ...]\n", argv[0]);
        return;
    }

    command_onchange = argv[1];

    while (argc-- > 2) {
        fprintf(stderr, "Adding watch on %s\n", argv[argc]);
        uv_fs_event_t *fs_event_req = malloc(sizeof(uv_fs_event_t));
        uv_fs_event_init(uv_default_loop(), fs_event_req);
        // The recursive flag watches subdirectories too.
        uv_fs_event_start(fs_event_req, run_command_onchange, argv[argc], UV_FS_EVENT_RECURSIVE);
    }

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}