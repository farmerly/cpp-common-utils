#include "TcpClient.h"
#include <arpa/inet.h>
#include <chrono>
#include <cstring>
#include <glog/logging.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#ifndef INVALID_SOCKET
    #define INVALID_SOCKET (-1)
#endif

using namespace sockutils;
using namespace std;
using namespace chrono;

TcpClient::TcpClient()
{
    m_isConnected = false;
    m_sockfd = INVALID_SOCKET;
    m_running = false;
    FD_ZERO(&m_connfds);
}

TcpClient::~TcpClient()
{
    if (m_running) {
        stop();
    }
}

bool TcpClient::connect(const char *ip, uint16_t port)
{
    struct sockaddr_in clnt_addr;
    bzero(&clnt_addr, sizeof(clnt_addr));

    clnt_addr.sin_family = AF_INET;
    clnt_addr.sin_addr.s_addr = inet_addr(ip);
    clnt_addr.sin_port = htons(port);

    m_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sockfd < 0) {
        LOG(ERROR) << "socket 创建失败! error: " << strerror(errno);
        return false;
    }

    struct timeval timeout = {3, 0};
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        LOG(ERROR) << "socket 设置非阻塞读失败! error: " << strerror(errno);
        goto err;
    }

    if (::connect(m_sockfd, (struct sockaddr *)&clnt_addr, sizeof(clnt_addr)) < 0) {
        goto err;
    }
    LOG(INFO) << "连接到 " << ip << ":" << port << " 服务器成功...";
    FD_SET(m_sockfd, &m_connfds);
    m_isConnected = true;
    return true;

err:
    closesocket();
    return false;
}

void TcpClient::closesocket()
{
    if (m_sockfd != INVALID_SOCKET) {
        FD_CLR(m_sockfd, &m_connfds);
        ::close(m_sockfd);
        m_sockfd = INVALID_SOCKET;
    }
    m_isConnected = false;
}

uint64_t TcpClient::curTimestamp()
{
    auto timeSinceEpoch = system_clock::now().time_since_epoch();
    return duration_cast<milliseconds>(timeSinceEpoch).count();
}

void TcpClient::loopThread(std::string ip, uint16_t port)
{
    while (m_running) {
        uint64_t bTime = 0;
        while (m_running && !connect(ip.c_str(), port)) {
            uint64_t eTime = curTimestamp();
            if (eTime >= (bTime + 1000 * 60)) {
                // 每隔1分钟记录一次日志
                LOG(INFO) << "连接服务器失败, " << ip << ":" << port;
                bTime = eTime;
            }
            this_thread::sleep_for(milliseconds(1000));
        }

        while (m_running && m_isConnected) {
            timeval timeout = {3, 0};
            fd_set  tempfds = m_connfds;

            int ret = select(m_sockfd + 1, &tempfds, nullptr, nullptr, &timeout);
            if (ret > 0 && FD_ISSET(m_sockfd, &tempfds)) {
                if (!m_callback) {
                    char buffer[1024] = {0};
                    int  rcvlen = ::recv(m_sockfd, buffer, 1024, 0);
                    if (rcvlen == 0) {
                        LOG(WARNING) << "服务器主动断开连接, " << ip << ":" << port;
                        closesocket();
                        break;
                    } else if (rcvlen < 0) {
                        if (errno != EINTR && errno != EAGAIN) {
                            LOG(ERROR) << "与服务器连接异常, 断开连接, " << ip << ":" << port << ", reason: " << strerror(errno);
                            closesocket();
                            break;
                        }
                    } else {
                        // todo
                    }
                } else {
                    if (!m_callback(m_sockfd)) {
                        LOG(WARNING) << "与服务器断开连接, " << ip << ":" << port;
                        closesocket();
                        break;
                    }
                }
            } else if (ret < 0) {
                LOG(WARNING) << "客户端连接异常, 断开连接: " << ip << ":" << port;
                closesocket();
                break;
            }
        }
    }
}

bool TcpClient::start(std::string ip, uint16_t port)
{
    if (!m_callback) {
        LOG(WARNING) << "未设置回调函数, 可使用 setRecvCallback 方法设置 recv 回调";
    }

    m_running = true;
    m_thread = thread(&TcpClient::loopThread, this, ip, port);
    return true;
}

void TcpClient::stop()
{
    m_running = false;
    if (m_thread.joinable())
        m_thread.join();
    closesocket();
}

void TcpClient::setRecvCallback(TcpClientRecvCB callback)
{
    m_callback = callback;
}

int TcpClient::sendMessage(const char *buf, int len)
{
    if (!m_isConnected) {
        return -1;
    }

    int nbytes = 0;
    do {
        int ret = ::send(m_sockfd, buf + nbytes, len - nbytes, MSG_NOSIGNAL);
        if (ret <= 0) {
            LOG(INFO) << "发送失败: " << m_sockfd << ", " << errno;
            if (errno == EINTR) {
                continue;
            }
            return ret;
        }
        nbytes += ret;
    } while (nbytes != len);
    return nbytes;
}