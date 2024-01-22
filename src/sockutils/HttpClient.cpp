#include "HttpClient.h"
#include "Logging.h"
#include <curl/curl.h>
#include <memory>

HttpClient::HttpClient()
{
    curl_global_init(CURL_GLOBAL_ALL);
}

HttpClient::~HttpClient()
{
    curl_global_cleanup();
}

size_t HttpClient::receiveCB(void *contents, size_t size, size_t nmemb, void *stream)
{
    std::string *str = (std::string *)stream;
    *str = std::string((char *)contents);
    return size * nmemb;
}

std::shared_ptr<curl_slist> HttpClient::createHeaders(std::vector<std::string> property)
{
    struct curl_slist *headers = nullptr;
    for (int i = 0; i < property.size(); i++) {
        headers = curl_slist_append(headers, property[i].c_str());
    }
    return std::shared_ptr<curl_slist>(headers, [](curl_slist *p) {
        if (p)
            curl_slist_free_all(p);
    });
}

std::shared_ptr<TCurlResponse> HttpClient::postMessage(std::string url, std::shared_ptr<curl_slist> headers, std::string req)
{
    auto pRes = std::make_shared<TCurlResponse>();

    // 设置参数
    CURL *curl = curl_easy_init();
    if (!curl) {
        snprintf(pRes->errmsg, sizeof(pRes->errmsg), "curl init failed!");
        pRes->success = false;
        return pRes;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, req.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, req.length());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers.get());
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 100);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 100);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&pRes->resText);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receiveCB);
    CURLcode retCode = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (retCode != CURLE_OK) {
        snprintf(pRes->errmsg, sizeof(pRes->errmsg), "curl_easy_perform failed: %s", curl_easy_strerror(retCode));
        pRes->success = false;
        return pRes;
    }
    pRes->success = true;
    return pRes;
}
