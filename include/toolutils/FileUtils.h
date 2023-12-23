#pragma once

#include <string>

/**
 * 从文件路径截取文件名
 * @param 完整文件路径
 * @return 返回文件名
 */
const char *const_basename(const char *filepath);

/**
 * 判断文件是否存在
 * @param path 文件路径
 * @return true/false
 */
bool file_is_exists(const char *filepath);

/**
 * 判断文件夹是否存在
 * @param path 文件路径
 * @return true/false
 */
bool path_is_exists(const char *directory);

/**
 * 检查目录是否存在, 不存在则创建
 * @param directory 文件夹路径
 */
void create_directory(const char *directory);

/**
 * 删除目标文件夹
 * @param directory 文件夹路径
 * @return true/false
 */
bool remove_directory(const char *directory);

/**
 * 删除目标文件
 * @param directory 文件路径
 * @return true/false
 */
bool remove_file(const char *filepath);

/**
 * 递归创建文件夹
 * @param directory 文件夹路径
 */
void create_directory_recurse(const std::string &directory);

/**
 * 递归删除目标路径下所有文件
 * @param directory 文件夹路径
 * @return true/false
 */
bool remove_directory_recurse(const char *directory);
