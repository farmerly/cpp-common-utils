#pragma once

#include <string>

#if defined(__GNUC__)
typedef void (*sighandler_t)(int);

/**
 * 信号处理
 */
void sig_process(int signo, sighandler_t handler);

/**
 * 守护模式运行程序
 */
void sig_handler(int signo);

/**
 * 守护模式运行程序
 */
void run_daemon();
#endif

/**
 * 线程休眠
 * @param msec 毫秒数
 */
void msleep(uint16_t msec);

/**
 * 十六进制数据转 hex
 * @param buf 十六进制数据
 * @param len 数据长度
 * @return hex 字符串
 */
std::string get_hex_string(const unsigned char *buf, unsigned int len);
