#ifndef _FILE_UTILS_H_H_H
#define _FILE_UTILS_H_H_H

#include <string>

/**
 * 从文件路径截取文件名
 * @param 完整文件路径
 * @return 返回文件名
 */
const char *const_basename(const char *filepath);

/**
 * 检查目录是否存在, 不存在则创建
 * @param directory 文件夹路径
 */
void create_directory(const char *directory);

/**
 * 递归创建文件夹
 * @param directory 文件夹路径
 */
void create_directory_recurse(const std::string &directory);

/**
 * 移除目标文件夹
 * @param directory 文件夹路径
 */
bool remove_directory_recurse(const char *directory);

#endif