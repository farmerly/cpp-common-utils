#pragma once

#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

#if defined(__GNUC__)
typedef void (*sighandler_t)(int);

/**
 * 信号处理
 * @param signo 系统信号
 * @param handler 信号处理回调
 */
void sig_process(int signo, sighandler_t handler);

/**
 * 守护模式运行程序
 */
void run_daemon();
#endif

/**
 * 十六进制数据转 hex
 * @param buf 十六进制数据
 * @param len 数据长度
 * @return hex 字符串
 */
std::string get_hex_string(const unsigned char *buf, unsigned int len);

/**
 * 浮点数按照精度转换为字符串类型
 * @param number 输入浮点型数字
 * @param limits 输出数字精度(整数加小数)
 * @return std::string 转换完成的字符串
 */
template <typename T>
std::string decimal2string(T number, int limits = 0)
{
    int digits = limits > 0 ? limits : std::numeric_limits<T>::digits10;

    // 数字转字符串
    std::ostringstream oss;
    oss.precision(digits);
    oss << std::setbase(10) << number;
    std::string strNum = oss.str();

    if (strNum.find('.') != std::string::npos) {
        std::string::reverse_iterator rit = strNum.rbegin();
        while (rit != strNum.rend()) {
            if (*rit != '0') {
                if (*rit == '.')
                    rit++;
                break;
            }
            rit++;
        }

        if (rit != strNum.rbegin()) {
            std::string::iterator it = rit.base();
            strNum.erase(it, strNum.end());
        }
    }
    return strNum;
}

/**
 * 计算 lambda 表达式耗时
 * @param func lambda 表达式
 * @return double 类型, 毫秒耗时
 */
template <typename F>
double calculate_cost_time(F const &func)
{
    auto begin = std::chrono::system_clock::now();
    func();
    auto end = std::chrono::system_clock::now();
    return std::chrono::duration<double, std::milli>(end - begin).count();
}

/**
 * 主机字节序转网络字节序
 * @param uint64_t 无符长整型数字
 */
unsigned long long htonll(unsigned long long val);

/**
 * 网络字节序转主机字节序
 * @param uint64_t 无符长整型数字
 */
unsigned long long ntohll(unsigned long long val);
