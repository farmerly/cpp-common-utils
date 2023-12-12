#pragma once

#include <functional>
#include <string>
#include <sys/select.h>
#include <thread>

namespace sockutils {

typedef std::function<bool(int sockfd)> TcpClientRecvCB;

class TcpClient
{
public:
    TcpClient();
    virtual ~TcpClient();

public:
    bool start(std::string ip, uint16_t port);
    void stop();
    void setRecvCallback(TcpClientRecvCB callback);
    int  sendMessage(const char *buf, int len);

private:
    bool     connect(const char *ip, uint16_t port);
    void     closesocket();
    uint64_t curTimestamp();
    void     loopThread(std::string ip, uint16_t port);

private:
    bool            m_isConnected;
    fd_set          m_connfds;
    int             m_sockfd;
    bool            m_running;
    std::thread     m_thread;
    TcpClientRecvCB m_callback;
};
} // namespace sockutils