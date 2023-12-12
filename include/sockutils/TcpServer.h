#pragma once

#include "TcpConnection.h"

#include <event2/event.h>
#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <thread>

namespace sockutils {

class TcpServer;

struct TTcpConnInfo {
    int            index;
    TcpConnection *connection;
    TcpServer     *server;
};

typedef std::function<bool(TcpConnection *)> EventConnectionCB;
typedef std::function<void(TcpConnection *)> EventMessageCB;

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
    void setConnectionCallback(EventConnectionCB callback);

    /**
     * 设置消息处理回调
     * @param callback 消息处理回调
     */
    void setMessageCallback(EventMessageCB callback);

private:
    bool               m_running; // 服务运行标识
    std::thread        m_thread;  // 线程句柄
    struct event_base *m_base;
    EventConnectionCB  m_connectionCB;
    EventMessageCB     m_messageCB;
    TTcpConnInfo      *m_connections;
    std::mutex         m_connLock;
};
} // namespace sockutils