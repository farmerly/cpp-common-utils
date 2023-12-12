#include "TimeUtils.h"
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
