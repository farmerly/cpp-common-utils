#include "TimeUtils.h"
#include "StringUtils.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

uint64_t get_current_timestamp_seconds()
{
    auto tp = std::chrono::high_resolution_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::seconds>(tp).count();
}

uint64_t get_current_timestamp_millis()
{
    auto tp = std::chrono::high_resolution_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(tp).count();
}

std::string get_current_format_datetime()
{
    std::stringstream ss;
    auto              tp = std::chrono::high_resolution_clock::now();
    time_t            timer = std::chrono::system_clock::to_time_t(tp);
    ss << std::put_time(std::localtime(&timer), "%F %T");
    auto millis = std::chrono::duration_cast<std::chrono::microseconds>(tp.time_since_epoch());
    auto second = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch());
    auto ms = (millis - second).count();
    ss << "." << std::setfill('0') << std::setw(6) << ms;
    return ss.str();
}

uint64_t datetime_to_timestamp(std::string datetime)
{
    uint16_t milliseconds = 0;
    auto     pos = datetime.find(".");
    if (pos != std::string::npos) {
        std::string ms = datetime.substr(pos + 1);
        milliseconds = atoi(ms.c_str());
        datetime = datetime.substr(0, pos);
    }
    if (datetime.find("T") != std::string::npos) {
        string_replace(datetime, "T", " ");
    }

    struct tm timeinfo = {0, 0, 0, 1, 0, 70};
    if (datetime.find(" ") != std::string::npos) {
        strptime(datetime.c_str(), "%Y-%m-%d %H:%M:%S", &timeinfo);
    } else {
        if (datetime.find("-") != std::string::npos) {
            std::vector<std::string> vecString = split_strings(datetime, "-");
            if (vecString.size() > 0)
                timeinfo.tm_year = std::atoi(vecString[0].c_str()) - 1900;
            if (vecString.size() > 1)
                timeinfo.tm_mon = std::atoi(vecString[1].c_str()) - 1;
            if (vecString.size() > 2)
                timeinfo.tm_mday = std::atoi(vecString[2].c_str());
        } else if (datetime.find(":") != std::string::npos) {
            std::vector<std::string> vecString = split_strings(datetime, ":");
            if (vecString.size() > 0)
                timeinfo.tm_hour = std::atoi(vecString[0].c_str());
            if (vecString.size() > 1)
                timeinfo.tm_min = std::atoi(vecString[1].c_str());
            if (vecString.size() > 2)
                timeinfo.tm_sec = std::atoi(vecString[2].c_str());
        }
    }

    time_t timer = mktime(&timeinfo);
    return (uint64_t)timer * 1000 + (uint64_t)milliseconds;
}
