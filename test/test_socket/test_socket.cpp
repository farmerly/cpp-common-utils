#include "test_socket.h"
#include "Logging.h"
#include "TcpClient.h"
#include "TcpConnection.h"
#include "TcpServer.h"
#include "UdpServer.h"
#include <chrono>
#include <string>
#include <thread>

using namespace std;

void test_tcp_client_run(const char *ip, uint16_t port)
{
    sockutils::TcpClient client;
    // client.setRecvCallback([](int fd) {
    //     char buffer[1024] = {0};
    //     int  ret = recv(fd, buffer, 1024, 0);
    //     if (ret <= 0) {
    //         return false;
    //     } else {
    //         LOG(INFO) << "收到数据: " << buffer;
    //     }
    //     return true;
    // });

    client.start(ip, port);

    std::string msg = "hello";
    int         index = 0;
    while (true) {
        std::string sndmsg = msg + ", " + to_string(index++) + "\n";
        if (client.sendMessage(sndmsg.c_str(), sndmsg.length()) > 0) {
            log_info("发送成功: %s", sndmsg.c_str());
        }
        this_thread::sleep_for(chrono::milliseconds(1));
    }
    this_thread::sleep_for(chrono::milliseconds(100));
    client.stop();
}

void test_tcp_server_run(uint16_t port)
{
    sockutils::TcpServer server;
    server.setConnectionCallback([](sockutils::TConnectionInfo *conn) {
        conn->readCallback = [](sockutils::TcpConnection *conn, void *args) {

        };
    });

    server.start(port);
    while (true) {
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    server.stop();
}

void test_udp_server_run(uint16_t port)
{
    sockutils::UdpServer udp;
}