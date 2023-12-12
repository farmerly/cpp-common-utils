#pragma once

#include "TcpClient.h"
#include "TcpServer.h"
#include "UdpServer.h"

struct TDataHandler {
    char buf[4096];
    int  len;
};

void test_tcp_client_run(const char *ip, uint16_t port);
void test_tcp_server_run(uint16_t port);
void test_udp_server_run(uint16_t port);