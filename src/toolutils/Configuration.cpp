#include "Configuration.h"
#include "StringUtils.h"
#include "TimeUtils.h"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>

bool set_config_prop_string(const char *filename, const char *key, const char *value)
{
    int   length = strlen(key) + strlen(value) + 10;
    char *property = new char[length];
    sprintf_safe(property, length, "%s = %s\n", key, value);
    std::ofstream ofile(filename, std::ios::app);
    if (!ofile.is_open()) {
        return false;
    }
    ofile.write(property, strlen(property));
    ofile.close();
    return true;
}

std::string get_config_prop_string(const char *filename, const char *key, const char *default_value)
{
    std::ifstream ifile(filename);
    if (ifile.is_open()) {
        int         position = 0;
        std::string linestr;
        while (std::getline(ifile, linestr)) {
            if (string_starts_with(linestr, "#"))
                continue;

            if ((position = linestr.find("=")) == linestr.npos)
                continue;

            std::string k = linestr.substr(0, position);
            if (strncmp(key, k.c_str(), strlen(key)) != 0)
                continue;

            ifile.close();
            std::string v = linestr.substr(position + 1, linestr.npos);
            return string_trim(v, ' ');
        }
        ifile.close();
    }

    printf("%s [warn]: Not Found [%s]\n", get_current_format_datetime().c_str(), key);
    if (default_value != nullptr) {
        set_config_prop_string(filename, key, default_value);
        return std::string(default_value);
    }
    return "";
}

int get_config_prop_int(const char *filename, const char *keyname, int default_value)
{
    std::string value_str = std::to_string(default_value);
    std::string config_value = get_config_prop_string(filename, keyname, value_str.c_str());
    return std::atoi(config_value.c_str());
}

double get_config_prop_double(const char *filename, const char *keyname, double default_value)
{
    std::string value_str = std::to_string(default_value);
    std::string config_value = get_config_prop_string(filename, keyname, value_str.c_str());
    return std::strtod(config_value.c_str(), nullptr);
}