#include "StringUtils.h"
#include <cstdarg>
#include <cstring>
#include <iomanip>
#include <list>
#include <sstream>

int sprintf_safe(char *dest, int size, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
#if defined(__GNUC__)
    int n = vsnprintf(dest, size, fmt, args);
#elif defined(_MSC_VER)
    int n = vsprintf_s(dest, size, fmt, args);
#endif
    va_end(args);
    return n;
}

std::vector<std::string> split_strings(const std::string &msg, std::string separator)
{
    std::vector<std::string> vecStrings;

    // start 起始位置, pos 分隔符位置
    int start = 0, pos = 0;
    while ((pos = msg.find(separator, start)) != std::string::npos) {
        if (start != pos)
            vecStrings.push_back(msg.substr(start, pos - start));
        start = pos + separator.length();
    }
    if (start != msg.length())
        vecStrings.push_back(msg.substr(start));
    return vecStrings;
}

std::string string_trim(const std::string &msg, const char ch)
{
    std::string message = msg;
    while (message.length() > 0) {
        if (message[0] != ch) {
            break;
        }
        message.erase(0, 1);
    }

    while (message.length() > 0) {
        if (message[message.length() - 1] != ch)
            break;
        message.erase(message.length() - 1, message.length());
    }
    return message;
}

void string_replace(std::string &msg, std::string src, std::string dst)
{
    while (msg.find(src) != std::string::npos) {
        msg.replace(msg.find(src), src.length(), dst);
    }
}

bool string_starts_with(const std::string &str, std::string start)
{
    auto startLen = start.size();
    if (str.size() >= startLen) {
        std::string temp = str.substr(0, startLen);
        return temp.compare(start) == 0;
    }
    return false;
}