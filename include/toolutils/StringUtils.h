#pragma once

#include <string>

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
std::string *get_split_strings(std::string msg, const char *separator, int &count);

/**
 * 释放 get_split_strings 的返回值
 */
void split_strings_free(std::string *&strings);

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