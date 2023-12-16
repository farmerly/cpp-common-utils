#pragma once

#include <string>
#include <vector>

/**
 * safe sprintf
 * @param dest
 * @param size bufsize
 * @param fmt 字符串格式化
 */
int sprintf_safe(char *dest, int size, const char *fmt, ...);

/**
 * 字符分割
 */
std::vector<std::string> split_strings(const std::string &msg, std::string separator);

/**
 * 去除左右两端字符
 */
void string_trim(std::string &msg, const char ch);

/**
 * 字符串替换
 * @param string msg 原始字符串
 * @param string src 替换前的字符
 * @param string dst 替换后的字符
 */
void string_replace(std::string &msg, std::string src, std::string dst);

/**
 * string 起始字符串判断
 */
bool string_starts_with(const std::string &str, std::string start);