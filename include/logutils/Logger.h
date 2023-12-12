#pragma once

#include "FileUtils.h"
#include "LogManager.h"
#include "TimeUtils.h"
#include <cstring>
#include <string>

extern logutils::LogManager *g_logManager;

#if defined(__GNUC__)
    #define log_debug(format, args...)                                                                                    \
        do {                                                                                                              \
            std::string datetime = get_current_format_datetime();                                                         \
            const char *basename = const_basename(__FILE__);                                                              \
            g_logManager->logRecord(                                                                                      \
                logutils::LOG_LEVEL_DEBUG, "[%s][%s:%d][D]: " format "\n", datetime.c_str(), basename, __LINE__, ##args); \
        } while (0)

    #define log_info(format, args...)                                                                                    \
        do {                                                                                                             \
            std::string datetime = get_current_format_datetime();                                                        \
            const char *basename = const_basename(__FILE__);                                                             \
            g_logManager->logRecord(                                                                                     \
                logutils::LOG_LEVEL_INFO, "[%s][%s:%d][I]: " format "\n", datetime.c_str(), basename, __LINE__, ##args); \
        } while (0)

    #define log_warn(format, args...)                                                                                    \
        do {                                                                                                             \
            std::string datetime = get_current_format_datetime();                                                        \
            const char *basename = const_basename(__FILE__);                                                             \
            g_logManager->logRecord(                                                                                     \
                logutils::LOG_LEVEL_WARN, "[%s][%s:%d][W]: " format "\n", datetime.c_str(), basename, __LINE__, ##args); \
        } while (0)

    #define log_error(format, args...)                                                                                    \
        do {                                                                                                              \
            std::string datetime = get_current_format_datetime();                                                         \
            const char *basename = const_basename(__FILE__);                                                              \
            g_logManager->logRecord(                                                                                      \
                logutils::LOG_LEVEL_ERROR, "[%s][%s:%d][E]: " format "\n", datetime.c_str(), basename, __LINE__, ##args); \
        } while (0)
#elif defined(_MSC_VER)
#endif

/**
 * 初始化全局 logManager
 * @param filename 文件名称
 * @param keepDays 日志留存时间
 * @return true/false
 */
bool init_log_manager(std::string filename, int level = 0, int keepDays = 0);