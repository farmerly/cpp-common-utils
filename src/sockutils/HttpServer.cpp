#include "HttpServer.h"
#include "Logging.h"
#include "TimeUtils.h"
#include <cstring>
#include <event2/buffer.h>
#include <event2/http.h>
#include <event2/keyvalq_struct.h>
#include <event2/thread.h>
#include <iostream>
#include <json/reader.h>
#include <json/value.h>
#include <memory>
#include <utility>

HttpServer::HttpServer(uint16_t port)
{
    m_nPort = port;
    m_evBase = nullptr;
    m_evHttp = nullptr;
}

HttpServer::~HttpServer()
{
}

void HttpServer::eventDispatch(uint16_t port)
{
    std::string hostname = "0.0.0.0";
    if (evhttp_bind_socket(m_evHttp, hostname.c_str(), m_nPort) != 0) {
        log_error("evhttp_bind_socket on %d failed!", m_nPort);
    }
    evhttp_set_timeout(m_evHttp, 30);
    evhttp_set_gencb(m_evHttp, httpCallback, this);
    log_info("HTTP 服务启动成功, http://%s:%d", hostname.c_str(), m_nPort);
    event_base_dispatch(m_evBase);
}

void HttpServer::httpSendHtmlReply(struct evhttp_request *request, int code, const std::string &reply)
{
    evkeyvalq *outhead = evhttp_request_get_output_headers(request);
    evhttp_add_header(outhead, "Content-Type", "text/html;charset=utf-8");
    httpSendReply(request, code, reply);
}

void HttpServer::httpSendJsonReply(struct evhttp_request *request, int code, const std::string &reply)
{
    evkeyvalq *outhead = evhttp_request_get_output_headers(request);
    evhttp_add_header(outhead, "Content-Type", "application/json;charset=utf-8");
    httpSendReply(request, code, reply);
}

void HttpServer::httpSendReply(struct evhttp_request *request, int code, const std::string &reply)
{
    evbuffer *outbuf = evhttp_request_get_output_buffer(request);
    if (outbuf == nullptr) {
        log_error("outbuf is nullptr!!!");
    }
    evbuffer_add(outbuf, reply.c_str(), reply.length());
    evhttp_send_reply(request, code, "", outbuf);
}

std::shared_ptr<char> HttpServer::getPostData(struct evhttp_request *request)
{
    evbuffer *inbuf = evhttp_request_get_input_buffer(request);
    size_t    bufsize = evbuffer_get_length(inbuf);

    std::shared_ptr<char> reqbuf(new char[bufsize + 1], std::default_delete<char[]>());
    if (evbuffer_remove(inbuf, reqbuf.get(), bufsize) == -1) {
        return nullptr;
    }
    return reqbuf;
}

void HttpServer::httpCallback(struct evhttp_request *request, void *args)
{
    HttpServer     *pthis = (HttpServer *)args;
    evhttp_cmd_type cmdType = evhttp_request_get_command(request);

    // 路由校验
    const char        *srcUri = evhttp_request_get_uri(request);
    struct evhttp_uri *evUri = evhttp_uri_parse(srcUri);
    const char        *path = evhttp_uri_get_path(evUri);
    std::string        strPath(path, strlen(path));

    // // header 解析
    // std::map<std::string, std::string> mapHeaders;
    // evkeyvalq                         *headers = evhttp_request_get_input_headers(request);
    // for (evkeyval *p = headers->tqh_first; p != NULL; p = p->next.tqe_next) {
    //     mapHeaders.insert(std::pair<std::string, std::string>(p->key, p->value));
    // }

    // 校验接口请求方式
    if (cmdType == EVHTTP_REQ_GET) {
        log_info("收到 GET 请求[%s]", strPath.c_str());
        auto handlerIter = pthis->m_mapGetController.find(strPath);
        if (handlerIter == pthis->m_mapGetController.end()) {
            if (cmdType == EVHTTP_REQ_GET && strcmp(path, "/") == 0) {
                httpSendHtmlReply(request, HTTP_OK, "<h1>Hello, World!</h1>");
            } else {
                httpSendHtmlReply(request, HTTP_NOTFOUND, "<h1>Not Found</h1>");
            }
            return;
        }

        Json::Value response;
        int         ret = handlerIter->second(response);
        httpSendJsonReply(request, ret, response.toStyledString());
        Json::Value reqJson;
    } else if (cmdType == EVHTTP_REQ_POST) {
        log_info("收到 POST 请求[%s]", strPath.c_str());
        auto handlerIter = pthis->m_mapPostController.find(strPath);
        if (handlerIter == pthis->m_mapPostController.end()) {
            httpSendHtmlReply(request, HTTP_NOTFOUND, "<h1>Not Found</h1>");
            return;
        }

        // 获取 post 方法参数
        std::shared_ptr<char> reqBuf = getPostData(request);
        if (!reqBuf) {
            httpSendHtmlReply(request, HTTP_BADREQUEST, "<h1>Bad Request</h1>");
            return;
        }

        log_info("%s: %s", strPath.c_str(), reqBuf.get());
        // 解析 json 数据
        Json::Value  reqJson;
        Json::Reader reader;
        if (!reader.parse(reqBuf.get(), reqJson)) {
            httpSendHtmlReply(request, HTTP_BADREQUEST, "<h1>Bad Request</h1>");
            return;
        }

        // 获取处理句柄
        Json::Value response;
        int         ret = handlerIter->second(reqJson, response);
        httpSendJsonReply(request, ret, response.toStyledString());
    } else {
        httpSendHtmlReply(request, HTTP_BADMETHOD, "<h1>Bad Method</h1>");
    }
}

bool HttpServer::start()
{
    evthread_use_pthreads();
    m_evBase = event_base_new();
    if (!m_evBase) {
        log_error("event_base_new failed!");
        return false;
    }
    m_evHttp = evhttp_new(m_evBase);
    if (!m_evHttp) {
        log_error("evhttp_new failed!");
        event_base_free(m_evBase);
        return false;
    }

    m_thread = std::thread(&HttpServer::eventDispatch, this, m_nPort);
    return true;
}

void HttpServer::stop()
{
    event_base_loopbreak(m_evBase);
    if (m_thread.joinable()) {
        m_thread.join();
    }
    if (m_evBase) {
        event_base_free(m_evBase);
    }
    if (m_evHttp) {
        evhttp_free(m_evHttp);
    }
}

bool HttpServer::setGetCallback(std::string route, HttpGetHandler handler)
{
    if (m_mapGetController.find(route) != m_mapGetController.end()) {
        return false;
    }
    m_mapGetController.insert(std::pair<std::string, HttpGetHandler>(route, handler));
    return true;
}

bool HttpServer::setPostCallback(std::string route, HttpPostHandler handler)
{
    if (m_mapPostController.find(route) != m_mapPostController.end()) {
        return false;
    }
    m_mapPostController.insert(std::pair<std::string, HttpPostHandler>(route, handler));
    return true;
}
