#pragma once

#include <event2/http.h>
#include <map>
#include <memory>
#include <string>

/**
 * HTTP 接口返回值
 */
struct THttpRequest {
    evhttp_cmd_type                    cmd;
    std::string                        uri; // 路由地址
    std::map<std::string, std::string> headers;
    std::map<std::string, std::string> reqQuery; // get 方法参数
    std::shared_ptr<char>              reqBody;  // post 方法参数
};

/**
 * HTTP 接口返回值
 */
struct THttpResponse {
    std::map<std::string, std::string> headers;
    std::string                        resText;
    int                                retCode;
};

/**
 * CURL 发送返回值
 */
struct TCurlResponse {
    bool        success = false;   // true, 成功; false, 失败
    std::string resText;           // 收到的应答消息
    char        errmsg[512] = {0}; // 失败日志
};