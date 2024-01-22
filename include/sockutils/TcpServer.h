#pragma once

#include "TcpConnection.h"

#include <event2/event.h>
#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <thread>

namespace sockutils {

struct TConnectionInfo;
class TcpServer;

typedef std::function<void(TConnectionInfo *)>       EventConnectCB;
typedef std::function<void(TConnectionInfo *)>       EventDisConnectCB;
typedef std::function<void(TcpConnection *, void *)> EventMessageCB;

struct TConnectionInfo {
    int            index;
    TcpServer     *server;
    TcpConnection *connection;
    EventMessageCB readCallback; // 消息回调
    void          *callbackArgs; // 回调参数
};

/*****
 * 异步单线程，非线程安全，谨慎修改及使用
 *****/
class TcpServer
{
public:
    TcpServer();
    virtual ~TcpServer();

private:
    static void listenCallback(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *args);
    static void readCallback(struct bufferevent *bev, void *args);
    static void eventCallback(struct bufferevent *bev, short events, void *args);
    void        eventDispatch(uint16_t port);

public:
    bool start(uint16_t port);
    void stop();

    /**
     * 设置连接回调
     * @param callback 连接回调
     */
    void setConnectionCallback(EventConnectCB callback);
    void setDisconnectCallback(EventDisConnectCB callback);
    void broadcastMessage(const char *buf, uint32_t len);

private:
    bool               m_running; // 服务运行标识
    std::thread        m_thread;  // 线程句柄
    struct event_base *m_base;
    EventConnectCB     m_connectionCB;
    EventDisConnectCB  m_disConnectCB;
    TConnectionInfo   *m_connections;
    std::mutex         m_connLock;
};
} // namespace sockutils