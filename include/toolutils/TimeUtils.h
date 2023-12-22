#ifndef _TIME_UTILS_H_H_H
#define _TIME_UTILS_H_H_H

#include <string>

/**
 * 获取当前系统时间戳
 * @return 当前时间戳(秒)
 */
uint64_t get_current_timestamp_seconds();

/**
 * 获取当前系统时间戳
 * @return 当前时间戳(毫秒)
 */
uint64_t get_current_timestamp_millis();

/**
 * 获取当前格式化后的时间戳
 * @return YYYY-MM-DD HH:mm:ss.SSSSSS
 */
std::string get_current_format_datetime();

/**
 * 字符串时间转为时间戳
 * @param datetime 日期时间
 * @return 毫秒时间戳
 */
uint64_t datetime_to_timestamp(std::string datetime);

#endif
