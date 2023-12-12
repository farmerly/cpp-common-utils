#include "Configuration.h"
#include "StringUtils.h"
#include "TimeUtils.h"
#include <cstring>
#include <fstream>

std::string get_config_prop_string(const char *value_name, const char *default_value, const char *filename)
{
    int           position = 0;
    std::string   value;
    std::ifstream ifile(filename);
    if (ifile.is_open()) {
        while (!ifile.eof()) {
            std::string s;
            getline(ifile, s);

            if (s.length() <= 0 || s[0] == '#')
                continue;
            position = s.find("=");
            if (position == s.npos)
                continue;
            std::string key = s.substr(0, position);
            value = s.substr(position + 1, s.npos);
            if (strncmp(value_name, key.c_str(), strlen(value_name)) != 0)
                continue;
            ifile.close();
            string_trim(value, ' ');
            return value;
        }
        ifile.close();
    }

    char property[512] = {0};
    printf("%s [warn]: Not Found [%s]\n", get_current_format_datetime().c_str(), value_name);
    sprintf_safe(property, 512, "%s = %s\n", value_name, default_value);

    std::ofstream ofile(filename, std::ios::app);
    ofile.write(property, strlen(property));
    ofile.close();
    value = default_value;
    return value;
}

std::string get_config_prop_string(const char *value_name, const char *default_value)
{
#if defined(__GNUC__)
    return get_config_prop_string(value_name, default_value, "./configure.ini");
#elif defined(_MSC_VER)
    return get_config_prop_string(value_name, default_value, ".\\configure.ini");
#endif
}

int get_config_prop_int(const char *value_name, const char *default_value, const char *filename)
{
    std::string config_value = get_config_prop_string(value_name, default_value, filename);
    return atoi(config_value.c_str());
}

int get_config_prop_int(const char *value_name, const char *default_value)
{
#if defined(__GNUC__)
    return get_config_prop_int(value_name, default_value, "./configure.ini");
#elif defined(_MSC_VER)
    return get_config_prop_int(value_name, default_value, ".\\configure.ini");
#endif
}