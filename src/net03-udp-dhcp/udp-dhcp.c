//
// Created by kiyon on 12/02/2018.
//

#include "udp-dhcp.h"
#include <stdio.h>
#include <uv.h>
#include <stdlib.h>

uv_loop_t *loop_udp;

uv_udp_t recv_socket;

uv_udp_t send_socket;

void alloc_buffer_udp(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}

void on_read_udp(uv_udp_t *req, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags) {
    if (nread < 0) {
        fprintf(stderr, "Read error %s\n", uv_err_name((int) nread));
        uv_close((uv_handle_t *) req, NULL);
        free(buf->base);
        return;
    }

    char sender[17] = {0};
    uv_ip4_name((const struct sockaddr_in *) addr, sender, 16);
    fprintf(stderr, "Receive from %s\n", sender);

    // ... DHCP specific code
    unsigned int *as_integer = (unsigned int *) buf->base;
    unsigned int ipbin = ntohl(as_integer[4]);
    unsigned char ip[4] = {0};
    for (int i = 0; i < 4; i++) {
        ip[i] = (unsigned char) ((ipbin >> i * 8) & 0xff);
    }

    fprintf(stderr, "Offered IP %d.%d.%d.%d\n", ip[3], ip[2], ip[1], ip[0]);

    free(buf->base);
    uv_udp_recv_stop(req);

}

uv_buf_t make_discover_msg() {
    uv_buf_t buffer;
    alloc_buffer_udp(NULL, 256, &buffer);
    memset(buffer.base, 0, buffer.len);

    // BOOTREQUEST
    buffer.base[0] = 0x1;
    // HTYPE ethernet
    buffer.base[1] = 0x1;
    // HLEN
    buffer.base[2] = 0x6;
    // HOPS
    buffer.base[3] = 0x0;
    // XID 4 bytes
    buffer.base[4] = (unsigned int) random();
    // SECS
    buffer.base[8] = 0x0;
    // FLAGS
    buffer.base[10] = 0x80;
    // CIADDR 12-15 is all zeros
    // YIADDR 16-19 is all zeros
    // SIADDR 20-23 is all zeros
    // GIADDR 24-27 is all zeros
    // CHADDR 28-43 is the MAC address, use your own
    buffer.base[28] = 0xa8;
    buffer.base[29] = 0x86;
    buffer.base[30] = 0xdd;
    buffer.base[31] = 0xa3;
    buffer.base[32] = 0x9a;
    buffer.base[33] = 0xe3;
    // SNAME 64 bytes zero
    // FILE 128 bytes zero
    // OPTIONS
    // - magic cookie
    buffer.base[236] = 99;
    buffer.base[237] = 123;
    buffer.base[238] = 83;
    buffer.base[239] = 99;

    // DHCP Message type
    buffer.base[240] = 53;
    buffer.base[241] = 1;
    buffer.base[242] = 1; // DHCPDISCOVER

    // DHCP Parameter request list
    buffer.base[243] = 55;
    buffer.base[244] = 4;
    buffer.base[245] = 1;
    buffer.base[246] = 3;
    buffer.base[247] = 15;
    buffer.base[248] = 6;

    return buffer;
}

void on_send_udp(uv_udp_send_t *req, int status) {
    if (status) {
        fprintf(stderr, "Send error %s\n", uv_strerror(status));
        return;
    }
}

void run_udp_dhcp() {
    loop_udp = uv_default_loop();

    uv_udp_init(loop_udp, &recv_socket);
    struct sockaddr_in recv_addr;
    uv_ip4_addr("0.0.0.0", 68, &recv_addr);
    uv_udp_bind(&recv_socket, (const struct sockaddr *) &recv_addr, UV_UDP_REUSEADDR);
    uv_udp_recv_start(&recv_socket, alloc_buffer_udp, on_read_udp);

    uv_udp_init(loop_udp, &send_socket);
    struct sockaddr_in broadcast_addr;
    uv_udp_bind(&send_socket, (const struct sockaddr *) &broadcast_addr, 0);
    uv_udp_set_broadcast(&send_socket, 1);

    uv_udp_send_t send_req;
    uv_buf_t discover_msg = make_discover_msg();

    struct sockaddr_in send_addr;
    uv_ip4_addr("255.255.255.255", 67, &send_addr);
    uv_udp_send(&send_req, &send_socket, &discover_msg, 1, (const struct sockaddr *) &send_addr, on_send_udp);

    uv_run(loop_udp, UV_RUN_DEFAULT);
}