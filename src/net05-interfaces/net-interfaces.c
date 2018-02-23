//
// Created by kiyon on 23/02/2018.
//

#include "net-interfaces.h"
#include <stdio.h>
#include <uv.h>

void run_net_interfaces() {
    char buf[512];
    uv_interface_address_t *info;
    int count, i;

    uv_interface_addresses(&info, &count);
    i = count;

    printf("Number of interfaces: %d\n", count);
    while (i--) {
        uv_interface_address_t interface = info[i];

        printf("Name: %s\n", interface.name);
        printf("Internal? %s\n", interface.is_internal ? "YES" : "NO");

        if (interface.address.address4.sin_family == AF_INET) {
            uv_ip4_name(&interface.address.address4, buf, sizeof(buf));
            printf("IPv4 address: %s\n", buf);

            uv_ip4_name(&interface.netmask.netmask4, buf, sizeof(buf));
            printf("IPv4 netmask: %s\n", buf);
        } else if (interface.address.address4.sin_family == AF_INET6) {
            uv_ip6_name(&interface.address.address6, buf, sizeof(buf));
            printf("IPv6 address: %s\n", buf);

            uv_ip6_name(&interface.netmask.netmask6, buf, sizeof(buf));
            printf("IPv6 netmask: %s\n", buf);
        }

        printf("\n");
    }
    uv_free_interface_addresses(info, count);
}

//int main() {
//    run_net_interfaces();
//    return 0;
//}