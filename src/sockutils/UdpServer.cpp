#include "UdpServer.h"
#include <arpa/inet.h>
#include <glog/logging.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace sockutils;

UdpServer::UdpServer()
{
    m_sockfd = -1;
}

UdpServer::~UdpServer()
{
    if (m_running)
        stop();
}

bool UdpServer::bindAddress(uint16_t port)
{
    m_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_sockfd < 0) {
        return false;
    }

    struct timeval tv = {3, 0};
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        goto err;
    }

    if (port) {
        struct sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        if (::bind(m_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            goto err;
        }
    }
    LOG(INFO) << "UdpServer 服务启动成功, 端口号: " << port;
    return true;

err:
    closesocket();
    return false;
}

void UdpServer::closesocket()
{
    if (m_sockfd != -1) {
        ::close(m_sockfd);
        m_sockfd = -1;
    }
}

void UdpServer::loopThread(uint16_t port)
{
    if (!bindAddress(port))
        return;

    char buffer[65535] = {0};
    while (m_running) {
        struct sockaddr_in sa;
        socklen_t          sa_size = sizeof(sa);

        int len = recvfrom(m_sockfd, buffer, 65535, 0, (sockaddr *)&sa, &sa_size);
        if (len > 0) {
            m_callback(buffer, len, &sa);
        } else if (len < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN)
                printf("recvfrom timeout\n");
            else
                printf("recvfrom err:%d\n", len);
        }
    }
}

bool UdpServer::start(uint16_t port)
{
    if (!m_callback)
        return false;

    m_running = true;
    m_thread = std::thread(&UdpServer::loopThread, this, port);
    return true;
}

void UdpServer::stop()
{
    m_running = false;
    if (m_thread.joinable()) {
        m_thread.join();
    }
    closesocket();
}

void UdpServer::setRecvCallback(UdpServerRecvCB callback)
{
    m_callback = callback;
}

int UdpServer::sendMessage(char *buf, int len, sockaddr_in *sa)
{
    return sendto(m_sockfd, buf, len, 0, (sockaddr *)&sa, sizeof(sockaddr_in));
}
