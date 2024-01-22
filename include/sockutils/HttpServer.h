#pragma once

#include <event2/event.h>
#include <event2/http.h>
#include <functional>
#include <json/json.h>
#include <map>
#include <memory>
#include <thread>

typedef std::function<int(Json::Value &)>                      HttpGetHandler;
typedef std::function<int(const Json::Value &, Json::Value &)> HttpPostHandler;

class HttpServer
{
public:
    HttpServer(uint16_t port);
    ~HttpServer();

private:
    void        eventDispatch(uint16_t port);
    static void httpCallback(struct evhttp_request *request, void *args);

private:
    static void httpSendReply(struct evhttp_request *request, int code, const std::string &reply);
    static void httpSendHtmlReply(struct evhttp_request *request, int code, const std::string &reply);
    static void httpSendJsonReply(struct evhttp_request *request, int code, const std::string &reply);

private:
    /**
     * 获取 Post 方法传输的数据
     * @param request http 请求 context
     * @return
     */
    static std::shared_ptr<char> getPostData(struct evhttp_request *request);

public:
    bool start();
    void stop();
    bool setGetCallback(std::string route, HttpGetHandler handler);
    bool setPostCallback(std::string route, HttpPostHandler handler);

private:
    uint16_t           m_nPort;
    struct event_base *m_evBase;
    struct evhttp     *m_evHttp;
    std::thread        m_thread;

private:
    std::map<std::string, HttpPostHandler> m_mapPostController;
    std::map<std::string, HttpGetHandler>  m_mapGetController;
};