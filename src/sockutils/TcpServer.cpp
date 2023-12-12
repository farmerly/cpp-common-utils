#include "TcpServer.h"
#include "TcpConnection.h"
#include <arpa/inet.h>
#include <cstddef>
#include <cstdint>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/thread.h>
#include <glog/logging.h>
#include <iomanip>
#include <mutex>
#include <sys/socket.h>
#include <thread>

using namespace sockutils;

static constexpr int max_connection = 32;

TcpServer::TcpServer()
{
    m_connections = new TTcpConnInfo[max_connection];
    for (int i = 0; i < max_connection; i++) {
        m_connections[i].index = i;
        m_connections[i].connection = nullptr;
        m_connections[i].server = nullptr;
    }
    m_base = nullptr;
    m_running = false;
}

TcpServer::~TcpServer()
{
    if (m_running) {
        stop();
    }
    if (m_connections) {
        delete[] m_connections;
    }
}

void TcpServer::listenCallback(struct evconnlistener *listener, evutil_socket_t sock, struct sockaddr *addr, int socklen, void *args)
{
    TcpServer          *pthis = (TcpServer *)args;
    struct bufferevent *bev;

    // 释放 bufferevent 时关闭底层传输端口
    bev = bufferevent_socket_new(pthis->m_base, sock, BEV_OPT_CLOSE_ON_FREE);
    if (!bev) {
        LOG(ERROR) << "Error constructing bufferevent!";
        event_base_loopbreak(pthis->m_base);
        return;
    }

    // 调用注册的回调方法
    TcpConnection *conn = new TcpConnection(bev, addr);
    if (pthis->m_connectionCB) {
        if (!pthis->m_connectionCB(conn)) {
            delete conn;
            return;
        }

        if (!conn->isConnected()) {
            delete conn;
            return;
        }
    }

    // 放入连接队列
    std::lock_guard<std::mutex> guard(pthis->m_connLock);
    for (int i = 0; i < max_connection; i++) {
        if (!pthis->m_connections[i].connection) {
            pthis->m_connections[i].server = pthis;
            pthis->m_connections[i].connection = conn;
            LOG(INFO) << "客户端 [" << conn->getPeerAddress()->toIpPort() << "] 连接成功.";
            bufferevent_setcb(bev, readCallback, NULL, eventCallback, &pthis->m_connections[i]);
            bufferevent_enable(bev, EV_READ);
            return;
        }
    }

    LOG(WARNING) << "超过最大连接数限制, 关闭客户端 [" << conn->getPeerAddress()->toIpPort() << "] 连接.";
    delete conn;
}

void TcpServer::readCallback(struct bufferevent *bev, void *args)
{
    TTcpConnInfo *connEvent = (TTcpConnInfo *)args;
    if (connEvent->server->m_messageCB) {
        connEvent->server->m_messageCB(connEvent->connection);
    } else {
        int ret = connEvent->connection->recvMessage();
        if (ret == 0) {
            connEvent->connection->disconnect();
        }
        // 丢弃数据
        connEvent->connection->getDataBuffer()->clear();
    }
}

void TcpServer::eventCallback(struct bufferevent *bev, short events, void *args)
{
    TTcpConnInfo *connEvent = static_cast<TTcpConnInfo *>(args);
    if (events & BEV_EVENT_EOF) {
        LOG(WARNING) << "客户端 [" << connEvent->connection->getPeerAddress()->toIpPort() << "] 连接断开.";
    } else if (events & BEV_EVENT_ERROR) {
        LOG(WARNING) << "客户端 [" << connEvent->connection->getPeerAddress()->toIpPort()
                     << "] 连接异常: " << evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR());
    }
    if (connEvent->connection) {
        delete connEvent->connection;
        connEvent->connection = nullptr;
    }
}

void TcpServer::eventDispatch(uint16_t port)
{
    struct sockaddr_in sock_addr = {0};
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons(port);

    ::evconnlistener *listener = evconnlistener_new_bind(m_base,
                                                         (evconnlistener_cb)listenCallback,
                                                         this,
                                                         LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
                                                         -1,
                                                         (struct sockaddr *)&sock_addr,
                                                         sizeof(sock_addr));
    if (!listener) {
        LOG(ERROR) << "端口监听失败，程序退出: " << evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR());
        exit(EXIT_FAILURE);
    }

    LOG(INFO) << "server listen on port: " << port;
    while (m_running) {
        int ret = event_base_dispatch(m_base);
        if (ret == -1) {
            LOG(ERROR) << "event_base_dispatch 异常，程序退出: " << evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR());
            break;
        }
    }

    // 关闭监听
    LOG(WARNING) << "服务端关闭监听, eventDispatch 线程退出";
    evconnlistener_free(listener);
    // 主动断开已建立连接的客户端
    for (int i = 0; i < max_connection; i++) {
        if (m_connections[i].connection) {
            delete m_connections[i].connection;
            m_connections[i].connection = nullptr;
        }
    }
}

bool TcpServer::start(uint16_t port)
{
    m_running = true;
    evthread_use_pthreads();
    m_base = event_base_new();
    m_thread = std::thread(&TcpServer::eventDispatch, this, port);
    return true;
}

void TcpServer::stop()
{
    m_running = false;
    event_base_loopbreak(m_base);
    if (m_thread.joinable()) {
        m_thread.join();
    }
    event_base_free(m_base);
}

void TcpServer::setConnectionCallback(EventConnectionCB callback)
{
    m_connectionCB = callback;
}

void TcpServer::setMessageCallback(EventMessageCB callback)
{
    m_messageCB = callback;
}