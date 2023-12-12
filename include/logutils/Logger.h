#pragma once

#include "FileUtils.h"
#include "LogManager.h"
#include "TimeUtils.h"
#include <cstring>
#include <string>

extern logutils::LogManager *g_logManager;

#if defined(__GNUC__)
    #define log_debug(format, args...)                                                                                \
        do {                                                                                                          \
            std::string datetime = get_current_format_datetime();                                                     \
            const char *basename = const_basename(__FILE__);                                                          \
            g_logManager->logRecord(0, "[%s][%s:%d][D]: " format "\n", datetime.c_str(), basename, __LINE__, ##args); \
        } while (0)

    #define log_info(format, args...)                                                                                 \
        do {                                                                                                          \
            std::string datetime = get_current_format_datetime();                                                     \
            const char *basename = const_basename(__FILE__);                                                          \
            g_logManager->logRecord(1, "[%s][%s:%d][D]: " format "\n", datetime.c_str(), basename, __LINE__, ##args); \
        } while (0)

    #define log_warn(format, args...)                                                                                 \
        do {                                                                                                          \
            std::string datetime = get_current_format_datetime();                                                     \
            const char *basename = const_basename(__FILE__);                                                          \
            g_logManager->logRecord(2, "[%s][%s:%d][W]: " format "\n", datetime.c_str(), basename, __LINE__, ##args); \
        } while (0)

    #define log_error(format, args...)                                                                                \
        do {                                                                                                          \
            std::string datetime = get_current_format_datetime();                                                     \
            const char *basename = const_basename(__FILE__);                                                          \
            g_logManager->logRecord(3, "[%s][%s:%d][E]: " format "\n", datetime.c_str(), basename, __LINE__, ##args); \
        } while (0)
#elif defined(_MSC_VER)
#endif

/**
 * 初始化全局 logManager
 */
bool init_log_manager(std::string filename, int keepDays = 0);