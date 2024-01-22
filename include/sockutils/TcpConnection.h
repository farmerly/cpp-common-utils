#pragma once

#include <event2/bufferevent.h>
#include <functional>
#include <string>

namespace sockutils {

class PeerAddress
{
public:
    PeerAddress(struct sockaddr *sa);
    ~PeerAddress();

public:
    std::string getIP();
    uint16_t    getPort();
    std::string toIpPort() const;

private:
    struct sockaddr m_addr;
    std::string     m_ip;
    uint16_t        m_port;
};

class TcpConnection
{
public:
    TcpConnection(struct bufferevent *bev, struct sockaddr *sa);
    virtual ~TcpConnection();

public:
    bool               isConnected();
    void               disconnect();
    const PeerAddress &getPeerAddress() const;
    int                recvMessage(char *buf, int len); // 接收消息, 方法不会返回负值
    int                sendMessage(const char *buf, int len);

private:
    struct bufferevent *m_bev;
    PeerAddress         m_peerAddress;
    bool                m_isConnected;
};
} // namespace sockutils