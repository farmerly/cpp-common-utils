#pragma once

#include <functional>
#include <string>
#include <sys/select.h>
#include <thread>

namespace sockutils {

typedef std::function<bool(int sockfd)> TcpClientRecvCB;
typedef std::function<bool(int sockfd)> TcpClientHandShark;

class TcpClient
{
public:
    TcpClient();
    virtual ~TcpClient();

public:
    bool start(std::string ip, uint16_t port);
    void stop();
    void setRecvCallback(TcpClientRecvCB callback);
    void setHandSharkCallback(TcpClientHandShark handShark);
    int  sendMessage(const char *buf, int len);
    bool isConnected();

private:
    bool     connect(const char *ip, uint16_t port);
    void     closesocket();
    uint64_t curTimestamp();
    void     loopThread(std::string ip, uint16_t port);

private:
    bool               m_isConnected;
    bool               m_isHandShark;
    fd_set             m_connfds;
    int                m_sockfd;
    bool               m_running;
    std::thread        m_thread;
    TcpClientRecvCB    m_callback;
    TcpClientHandShark m_handShark; // 握手回调操作
};
} // namespace sockutils