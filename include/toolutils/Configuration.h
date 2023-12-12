#pragma once

#include <string>

/**
 * 获取配置
 */
std::string get_config_prop_string(const char *value_name, const char *default_value, const char *filename);

/**
 * 获取配置
 */
std::string get_config_prop_string(const char *value_name, const char *default_value);

/**
 * 获取配置
 */
int get_config_prop_int(const char *value_name, const char *default_value, const char *filename);

/**
 * 获取配置
 */
int get_config_prop_int(const char *value_name, const char *default_value);
