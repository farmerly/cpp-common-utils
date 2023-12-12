#pragma once

#include <event2/bufferevent.h>
#include <functional>
#include <string>

namespace sockutils {

class DataBuffer
{
public:
    DataBuffer(int maxSize = 4096);
    virtual ~DataBuffer();

public:
    unsigned char *buffer();
    unsigned int   length();
    void           length(unsigned int len);
    unsigned int   remain();
    int            memcpy(unsigned char *dst, unsigned int len);
    void           clear();

private:
    unsigned char *m_buffer;
    unsigned int   m_length;
    unsigned int   m_maxSize;
};

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
    TcpConnection(struct bufferevent *bev, struct sockaddr *sa, uint32_t maxSize = 4096);
    virtual ~TcpConnection();

public:
    bool         isConnected();
    void         disconnect();
    PeerAddress *getPeerAddress() const;
    DataBuffer  *getDataBuffer() const;
    int          recvMessage(); // 接收消息, 方法不会返回负值
    int          sendMessage(const char *buf, int len);

private:
    struct bufferevent *m_bev;
    PeerAddress        *m_peerAddress;
    DataBuffer         *m_dataBuffer;
    bool                m_isConnected;
};
} // namespace sockutils