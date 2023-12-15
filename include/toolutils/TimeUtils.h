#ifndef _TIME_UTILS_H_H_H
#define _TIME_UTILS_H_H_H

#include <string>

/**
 * 获取当前秒时间
 */
uint64_t get_current_timestamp_seconds();

/**
 * 获取当前毫秒时间
 */
uint64_t get_current_timestamp_millis();

/**
 * 获取当前格式化后的时间戳
 */
std::string get_current_format_datetime();

/**
 * 字符串时间转为时间戳
 */
uint64_t datetime_to_timestamp(std::string datetime);

#endif