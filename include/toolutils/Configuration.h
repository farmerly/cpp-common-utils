#pragma once

#include <string>

/**
 * 写入配置文件
 * @param key 键名称
 * @param value 值
 * @param filename 文件名
 * @return true/false
 */
bool set_config_prop_string(const char *filename, const char *key, const char *value);

/**
 * 读取配置文件
 * @param key 键值名称
 * @param default_value 默认值
 * @param filename 配置文件
 * @return 返回配置值
 */
std::string get_config_prop_string(const char *filename, const char *key, const char *default_value = nullptr);

/**
 * 获取配置值
 * @param key 键值名称
 * @param default_value 默认值
 * @param filename 配置文件
 * @return 返回配置值
 */
int get_config_prop_int(const char *filename, const char *key, int default_value = 0);

/**
 * 获取配置值
 * @param key 键值名称
 * @param default_value 默认值
 * @param filename 配置文件
 * @return 返回配置值
 */
double get_config_prop_double(const char *filename, const char *key, double default_value = 0.0);
