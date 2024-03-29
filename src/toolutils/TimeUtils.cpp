#include "TimeUtils.h"
#include "StringUtils.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std::chrono;

uint64_t get_current_timestamp_seconds()
{
    auto tp = high_resolution_clock::now().time_since_epoch();
    return duration_cast<seconds>(tp).count();
}

uint64_t get_current_timestamp_millis()
{
    auto tp = high_resolution_clock::now().time_since_epoch();
    return duration_cast<milliseconds>(tp).count();
}

std::string get_current_format_datetime()
{
    auto   tp = high_resolution_clock::now();
    time_t timer = system_clock::to_time_t(tp);

    std::stringstream ss;
    char              datetime[32] = {0};
    std::strftime(datetime, 32, "%Y-%m-%d %H:%M:%S", std::localtime(&timer));
    ss << datetime;

    auto micros = duration_cast<microseconds>(tp.time_since_epoch());
    auto second = duration_cast<seconds>(tp.time_since_epoch());
    auto microsec = (micros - second).count();
    ss << "." << std::setfill('0') << std::setw(6) << microsec;
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

std::string timestamp_to_datetime(time_t timer)
{
    char datetime[32] = {0};
    std::strftime(datetime, 32, "%Y-%m-%d %H:%M:%S", std::localtime(&timer));
    return std::string(datetime);
}

std::string milliseconds_to_datetime(uint64_t timestamp, bool hasMillis)
{
    system_clock::time_point tp{duration_cast<system_clock::time_point::duration>(milliseconds(timestamp))};
    time_t                   timer = system_clock::to_time_t(tp);

    std::stringstream ss;
    char              datetime[32] = {0};
    std::strftime(datetime, 32, "%Y-%m-%d %H:%M:%S", std::localtime(&timer));
    ss << datetime;
    if (hasMillis) {
        auto millis = duration_cast<milliseconds>(tp.time_since_epoch());
        auto second = duration_cast<seconds>(tp.time_since_epoch());
        auto millisec = (millis - second).count();
        ss << "." << std::setfill('0') << std::setw(3) << millisec;
    }
    return ss.str();
}