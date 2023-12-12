#include "test_socket.h"
#include <cstring>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc <= 2) {
        std::cout << "usage: " << argv[0] << " [client/server]" << std::endl;
        return -1;
    }
    if (memcmp(argv[1], "client", strlen(argv[1])) == 0) {
        std::string ip = argv[2];
        uint16_t    port = std::atoi(argv[3]);
        test_tcp_client_run(ip.c_str(), port);
    } else if (memcmp(argv[1], "server", strlen(argv[1])) == 0) {
        uint16_t port = std::atoi(argv[2]);
        test_tcp_server_run(port);
    } else {
        std::cout << "unknown" << std::endl;
    }
    return 0;
}