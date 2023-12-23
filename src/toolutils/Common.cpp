// =====================================================================================
//	Copyright (C) 2018 by Jiaxing Shao.All rights reserved.
//
//	文 件 名:  common.cpp
//	作    者:  Jiaxing Shao, 13315567369@163.com
//	版 本 号:  1.0
//	创建时间:  2020年06月08日 14时10分10秒
//	Compiler:  g++
//	描    述:
// =====================================================================================

#include "Common.h"
#include <stdlib.h>

#if defined(__GNUC__)
    #include <arpa/inet.h>
    #include <dirent.h>
    #include <signal.h>
    #include <stdarg.h>
    #include <sys/stat.h>
    #include <unistd.h>
#elif defined(_MSC_VER)
    #ifdef _WIN32
        #ifndef WIN32_LEAN_AND_MEAN
            #define WIN32_LEAN_AND_MEAN
        #endif //! WIN32_LEAN_AND_MEAN
        #include <Windows.h>
        #include <Winsock2.h>
        #include <process.h>
        #pragma comment(lib, "Ws2_32.lib")
        #undef WIN32_LEAN_AND_MEAN
    #endif //!_WIN32
#endif

using namespace std;

// hex 转换表
static constexpr char hex_table[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

#if defined(__GNUC__)
void sig_process(int signo, sighandler_t handler)
{
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(signo, &sa, NULL);
}

void run_daemon()
{
    pid_t pid = fork();
    if (pid < 0) {
        exit(1);
    } else if (pid > 0) {
        exit(0);
    }

    setsid();
    sig_process(SIGHUP, SIG_IGN);
    sig_process(SIGTERM, SIG_IGN);
    sig_process(SIGPIPE, SIG_IGN);
}

#endif

std::string get_hex_string(const unsigned char *buf, unsigned int len)
{
    if (buf == nullptr)
        return "";

    std::string message;
    for (int i = 0; i < len; i++) {
        message += hex_table[buf[i] >> 4];
        message += hex_table[buf[i] & 0x0F];
        if (len > i + 1)
            message += ' ';
    }
    return message;
}

unsigned long long htonll(unsigned long long val)
{
    if (__BYTE_ORDER == __LITTLE_ENDIAN) {
        return (((unsigned long long)htonl((int)((val << 32) >> 32))) << 32) | (unsigned int)htonl((int)(val >> 32));
    } else if (__BYTE_ORDER == __BIG_ENDIAN) {
        return val;
    }
}

unsigned long long ntohll(unsigned long long val)
{
    if (__BYTE_ORDER == __LITTLE_ENDIAN) {
        return (((unsigned long long)ntohl((int)((val << 32) >> 32))) << 32) | (unsigned int)ntohl((int)(val >> 32));
    } else if (__BYTE_ORDER == __BIG_ENDIAN) {
        return val;
    }
}