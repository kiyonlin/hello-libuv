cmake_minimum_required(VERSION 3.9)
project(hello_libuv C)

set(CMAKE_C_STANDARD 99)
link_libraries(/usr/local/Cellar/libuv/HEAD-c431505/lib/libuv.a)
include_directories(/usr/local/Cellar/libuv/HEAD-c431505/include)

set(SOURCE_FILES
        main.c
        src/basic01-helloworld/helloworld.c src/basic01-helloworld/helloworld.h
        src/file01-idle-basic/idle_basic.h src/file01-idle-basic/idle_basic.c
        src/file02-uvcat/uvcat.c src/file02-uvcat/uvcat.h
        src/thread01-uvonce/thread-uvonce.c src/thread01-uvonce/thread-uvonce.h
        src/thread02-mutex/thread-mutex.c src/thread02-mutex/thread-mutex.h
        src/thread03-rwlock/thread-rwlock.c src/thread03-rwlock/thread-rwlock.h
        src/thread04-binary-sem/thread-binary-sem.c src/thread04-binary-sem/thread-binary-sem.h
        src/thread05-multi-sem/thread-multi-sem.c src/thread05-multi-sem/thread-multi-sem.h
        src/thread06-cond/thread-cond.c src/thread06-cond/thread-cond.h
        src/thread07-barrier/thread-barrier.c src/thread07-barrier/thread-barrier.h
        src/thread08-tls/thread-tls.c src/thread08-tls/thread-tls.h
        src/net01-tcp-echo-server/tcp-echo-server.c src/net01-tcp-echo-server/tcp-echo-server.h
        src/net02-tcp-echo-client/tcp-echo-client.c src/net02-tcp-echo-client/tcp-echo-client.h
        src/net03-udp-dhcp/udp-dhcp.c src/net03-udp-dhcp/udp-dhcp.h
        src/net04-dns/dns.c src/net04-dns/dns.h
        src/net05-interfaces/net-interfaces.c src/net05-interfaces/net-interfaces.h
        src/process01-spawn/process-spawn.c src/process01-spawn/process-spawn.h
        src/process02_detach/process_detach.c src/process02_detach/process_detach.h
        src/process03_signals/process_signal.c src/process03_signals/process_signal.h
        src/process04-proc-streams/process-proc-streams.c src/process04-proc-streams/process-proc-streams.h
        src/process05-cgi/process-cgi.c src/process05-cgi/process-cgi.h
        src/process06-pipes/process-pipe.c src/process06-pipes/process-pipe.h
        src/process07-multi-echo-server/process-multi-echo-server.c src/process07-multi-echo-server/process-multi-echo-server.h
        src/basic02-uvstop/basic-uvstop.c src/basic02-uvstop/basic-uvstop.h
        src/utils01-timers/utils-ref-timer.c src/utils01-timers/utils-ref-timer.h
        src/utils02-idle-compute/utils-idle-compute.c src/utils02-idle-compute/utils-idle-compute.h
        src/thread09-queue-work/thread-queue-work.c src/thread09-queue-work/thread-queue-work.h
        src/thread10-queue-cancel/thread-queue-cancel.c src/thread10-queue-cancel/thread-queue-cancel.h
        src/thread11-progress/thread-progress.c src/thread11-progress/thread-progress.h
        src/file03-onchange/file-onchange.c src/file03-onchange/file-onchange.h
        src/file04-uvtee/file-uvtee.c src/file04-uvtee/file-uvtee.h)

add_executable(hello_libuv ${SOURCE_FILES})