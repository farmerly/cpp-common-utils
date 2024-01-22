#pragma once

#include "HttpConstants.h"
#include <curl/curl.h>
#include <memory>
#include <string>
#include <vector>

class HttpClient
{
public:
    HttpClient();
    virtual ~HttpClient();

public:
    // 创建 http 头
    std::shared_ptr<curl_slist> createHeaders(std::vector<std::string> property);
    // post方法发送数据
    std::shared_ptr<TCurlResponse> postMessage(std::string url, std::shared_ptr<curl_slist> headers, std::string req);

protected:
    static size_t receiveCB(void *contents, size_t size, size_t nmemb, void *stream);
};