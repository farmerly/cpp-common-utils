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
}

std::string *get_split_strings(std::string msg, const char *separator, int &count)
{
    std::list<std::string> string_list;
    int                    position = 0;
    count = 0;
    while (true) {
        int         n = msg.find(separator, position);
        std::string tmp_string = msg.substr(position, n - position);
        if (tmp_string.length() != 0) {
            string_list.push_back(tmp_string);
            count += 1;
        }
        if (n == -1)
            break;
        position = n + strlen(separator);
    }
    std::string                     *strings = new std::string[count];
    std::list<std::string>::iterator iter;
    int                              i = 0;
    for (iter = string_list.begin(); iter != string_list.end(); iter++) {
        strings[i++] = (*iter);
    }
    return strings;
}

void split_strings_free(std::string *&strings)
{
    if (strings == nullptr) {
        delete[] strings;
        strings = nullptr;
    }
}

void string_trim(std::string &msg, const char ch)
{
    while (msg.length() > 0) {
        if (msg[0] != ch) {
            break;
        }
        msg.erase(0, 1);
    }

    while (msg.length() > 0) {
        if (msg[msg.length() - 1] != ch)
            break;
        msg.erase(msg.length() - 1, msg.length());
    }
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