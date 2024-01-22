#include "TcpConnection.h"
#include <arpa/inet.h>
#include <cstring>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <string>

using namespace sockutils;

PeerAddress::PeerAddress(sockaddr *sa)
{
    memset(&m_addr, 0, sizeof(sockaddr));
    memcpy(&m_addr, sa, sizeof(sockaddr));
    m_ip = inet_ntoa(((struct sockaddr_in *)sa)->sin_addr);
    m_port = ntohs(((struct sockaddr_in *)sa)->sin_port);
}

PeerAddress::~PeerAddress()
{
}

std::string PeerAddress::getIP()
{
    return m_ip;
}

uint16_t PeerAddress::getPort()
{
    return m_port;
}

std::string PeerAddress::toIpPort() const
{
    return m_ip + ":" + std::to_string(m_port);
}

TcpConnection::TcpConnection(bufferevent *bev, sockaddr *sa) : m_peerAddress(sa)
{
    m_bev = bev;
    m_isConnected = true;
}

TcpConnection::~TcpConnection()
{
    if (isConnected()) {
        disconnect();
    }
}

bool TcpConnection::isConnected()
{
    return m_isConnected;
}

void TcpConnection::disconnect()
{
    if (m_isConnected && m_bev) {
        m_isConnected = false;
        bufferevent_free(m_bev);
        m_bev = nullptr;
    }
}

const PeerAddress &TcpConnection::getPeerAddress() const
{
    return m_peerAddress;
}

int TcpConnection::recvMessage(char *buf, int len)
{
    return ::bufferevent_read(m_bev, buf, len);
}

int TcpConnection::sendMessage(const char *buf, int len)
{
    if (m_isConnected && m_bev) {
        return ::bufferevent_write(m_bev, buf, len);
    }
    return -1;
}
