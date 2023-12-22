#pragma once

#include <arpa/inet.h>
#include <cstdint>
#include <functional>
#include <sys/socket.h>
#include <thread>

namespace sockutils {

typedef std::function<void(char *, int, sockaddr_in *)> UdpServerRecvCB;

class UdpServer
{
public:
    UdpServer();
    virtual ~UdpServer();

private:
    bool bindAddress(uint16_t port);
    void closesocket();
    void loopThread(uint16_t port);

public:
    bool start(uint16_t port);
    void stop();
    void setRecvCallback(UdpServerRecvCB callback);
    int  sendMessage(char *buf, int len, const sockaddr_in &sa);

private:
    int             m_sockfd;
    bool            m_running;
    std::thread     m_thread;
    UdpServerRecvCB m_callback;
};
} // namespace sockutils