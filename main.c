#include "src/basic01-helloworld/helloworld.h"
#include "src/file01-idle-basic/idle_basic.h"
#include "src/file02-uvcat/uvcat.h"
#include "src/net01-tcp-echo-server/tcp-echo-server.h"
#include "src/net02-tcp-echo-client/tcp-echo-client.h"
#include "src/net03-udp-dhcp/udp-dhcp.h"
#include "src/net04-dns/dns.h"
#include "src/net05-interfaces/net-interfaces.h"
#include "src/thread01-uvonce/thread-uvonce.h"
#include "src/thread02-mutex/thread-mutex.h"
#include "src/thread03-rwlock/thread-rwlock.h"
#include "src/thread04-binary-sem/thread-binary-sem.h"
#include "src/thread05-multi-sem/thread-multi-sem.h"
#include "src/thread06-cond/thread-cond.h"
#include "src/thread07-barrier/thread-barrier.h"
#include "src/thread08-tls/thread-tls.h"
#include "src/process01-spawn/process-spawn.h"
#include "src/process02_detach/process_detach.h"
#include "src/process03_signals/process_signal.h"

int main(int argc, char** argv) {
//    run_hello_world();

//    run_idle_basic();

//    run_uvcat(argc, argv);

    // 配合客户端使用
//    run_tcp_echo_server();
    // 配个服务端使用
//    run_tcp_echo_client();
//    run_udp_dhcp();
//    run_dns();
//    run_net_interfaces();

//    run_process_spawn();
//    run_process_detach();

    // 调用`kill -USR1 36669`触发
    run_process_signal();

//    run_thread_uvonce();
//    run_thread_mutex();
//    run_thread_rwlock();
//    run_thread_binary_sem();
//    run_thread_multi_sem();
//    run_thread_cond();
//    run_thread_barrier();
//    run_thread_tls();

    return 0;
}