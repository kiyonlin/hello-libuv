cmake_minimum_required(VERSION 3.9)
project(hello_libuv C)

set(CMAKE_C_STANDARD 99)
link_libraries(/usr/local/Cellar/libuv/HEAD-c431505/lib/libuv.a)
include_directories(/usr/local/Cellar/libuv/HEAD-c431505/include)

set(SOURCE_FILES
        main.c
        src/01helloworld/helloworld.c src/01helloworld/helloworld.h
        src/02idle-basic/idle_basic.h src/02idle-basic/idle_basic.c)

add_executable(hello_libuv ${SOURCE_FILES})