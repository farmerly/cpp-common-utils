#pragma once

#include "LogConstants.h"
#include <cstdarg>

namespace logutils {
class LogMessage
{
public:
    LogMessage();
    virtual ~LogMessage();

public:
    char *getMessage();
    int   setMessage(int level, const char *format, va_list &args);
    int   length();
    int   level();

private:
    int   m_level;
    char *m_buffer;
    int   m_length;
};
} // namespace logutils