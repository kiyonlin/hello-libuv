cmake_minimum_required(VERSION 3.9)
project(hello_libuv C)

set(CMAKE_C_STANDARD 99)
link_libraries(/usr/local/Cellar/libuv/HEAD-c431505/lib/libuv.a)
include_directories(/usr/local/Cellar/libuv/HEAD-c431505/include)

set(SOURCE_FILES
        main.c
        src/01helloworld/helloworld.c src/01helloworld/helloworld.h
        src/02idle-basic/idle_basic.h src/02idle-basic/idle_basic.c
        src/03uvcat/uvcat.c src/03uvcat/uvcat.h
        src/thread01-uvonce/thread-uvonce.c src/thread01-uvonce/thread-uvonce.h
        src/thread02-mutex/thread-mutex.c src/thread02-mutex/thread-mutex.h
        src/thread03-rwlock/thread-rwlock.c src/thread03-rwlock/thread-rwlock.h
        src/thread04-binary-sem/thread-binary-sem.c src/thread04-binary-sem/thread-binary-sem.h
        src/thread05-multi-sem/thread-multi-sem.c src/thread05-multi-sem/thread-multi-sem.h src/thread06-cond/thread-cond.c src/thread06-cond/thread-cond.h src/thread07-barrier/thread-barrier.c src/thread07-barrier/thread-barrier.h src/thread08-tls/thread-tls.c src/thread08-tls/thread-tls.h)

add_executable(hello_libuv ${SOURCE_FILES})