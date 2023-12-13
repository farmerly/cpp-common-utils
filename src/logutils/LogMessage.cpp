#include "LogMessage.h"
#include <cstdarg>
#include <cstring>
#include <string>

namespace logutils {

LogMessage::LogMessage()
{
    m_length = 0;
    m_buffer = new char[LOG_LINE_SIZE];
}

LogMessage::~LogMessage()
{
    if (m_buffer)
        delete[] m_buffer;
}

char *LogMessage::getMessage()
{
    return m_buffer;
}

int LogMessage::setMessage(int level, const char *format, va_list &args)
{
#if defined(__GNUC__)
    vsnprintf(m_buffer, LOG_LINE_SIZE, format, args);
#elif defined(_MSC_VER)
    vsprintf_s(log->message, format, args);
#endif

    m_level = level;
    m_length = strlen(m_buffer);
    return m_length;
}

int LogMessage::length()
{
    return m_length;
}

int LogMessage::level()
{
    return m_level;
}

} // namespace logutils