#include "TcpConnection.h"
#include <arpa/inet.h>
#include <cstring>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <glog/logging.h>
#include <string>

using namespace sockutils;

DataBuffer::DataBuffer(int maxSize)
{
    m_buffer = new unsigned char[maxSize];
    m_maxSize = maxSize;
    m_length = 0;
}

DataBuffer::~DataBuffer()
{
    if (m_buffer != nullptr)
        delete[] m_buffer;
}

unsigned char *DataBuffer::buffer()
{
    return m_buffer;
}

unsigned int DataBuffer::length()
{
    return m_length;
}

void DataBuffer::length(unsigned int len)
{
    m_length = len;
}

unsigned int DataBuffer::remain()
{
    return m_maxSize - m_length;
}

int DataBuffer::memcpy(unsigned char *dst, unsigned int len)
{
    if (len > m_length) {
        return -1;
    }

    ::memcpy(dst, m_buffer, len);
    ::memcpy(m_buffer, m_buffer + len, m_length - len);
    m_length -= len;
    return len;
}

void DataBuffer::clear()
{
    m_length = 0;
}

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

TcpConnection::TcpConnection(bufferevent *bev, sockaddr *sa, uint32_t maxSize)
{
    m_bev = bev;
    m_isConnected = true;
    m_peerAddress = new PeerAddress(sa);
    m_dataBuffer = new DataBuffer(maxSize);
}

TcpConnection::~TcpConnection()
{
    if (isConnected()) {
        disconnect();
    }
    if (m_peerAddress) {
        delete m_peerAddress;
    }
    if (m_dataBuffer) {
        delete m_dataBuffer;
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

PeerAddress *TcpConnection::getPeerAddress() const
{
    return m_peerAddress;
}

DataBuffer *TcpConnection::getDataBuffer() const
{
    return m_dataBuffer;
}

int TcpConnection::recvMessage()
{
    unsigned char *buffer = m_dataBuffer->buffer();
    uint32_t       remain = m_dataBuffer->remain();
    uint32_t       length = m_dataBuffer->length();

    int n = ::bufferevent_read(m_bev, buffer + length, remain);
    m_dataBuffer->length(length + n);
    return n;
}

int TcpConnection::sendMessage(const char *buf, int len)
{
    if (m_isConnected && m_bev) {
        return ::bufferevent_write(m_bev, buf, len);
    }
    return -1;
}
