#pragma once

#include <cstdarg>
#include <fstream>
#include <iostream>
#include <string>

namespace logutils {

static constexpr int LOG_TYPE_DEBUG = 0;
static constexpr int LOG_TYPE_INFO = 1;
static constexpr int LOG_TYPE_WARN = 2;
static constexpr int LOG_TYPE_ERROR = 3;

static constexpr int LOG_LINE_SIZE = 1024 * 16; // 单行日志大小
static constexpr int LOG_QUEUE_SIZE = 1024 * 8; // Log 队列长度

struct LogMessage {
    char message[LOG_LINE_SIZE];
};

class LogManager
{
public:
    LogManager();
    virtual ~LogManager();

public:
    void logRecord(int type, const char *format, ...)
    {
        char logContext[4096] = {0};

        va_list args;
        va_start(args, format);
#if defined(__GNUC__)
        vsnprintf(logContext, 4096, format, args);
#elif defined(_MSC_VER)
        vsprintf_s(logContext, format, args);
#endif
        va_end(args);

        if (type == LOG_TYPE_DEBUG) {
            printf("%s", logContext);
        } else if (type == LOG_TYPE_INFO) {
            printf("%s", logContext);
        } else if (type == LOG_TYPE_WARN) {
            printf("%s", logContext);
        } else if (type == LOG_TYPE_ERROR) {
            printf("%s", logContext);
        }
    }

private:
    inline LogMessage *getConsumer()
    {
        if (m_producer != m_consumer) {
            return &m_logQueue[m_consumer];
        }
        return nullptr;
    }

    inline void putConsumer()
    {
        m_consumer = (m_consumer + 1) % LOG_QUEUE_SIZE;
    }

    inline LogMessage *getProducer()
    {
        if (((m_producer + 1) % LOG_QUEUE_SIZE) != m_consumer) {
            return &m_logQueue[m_producer];
        }
        return nullptr;
    }

    inline void putProducer()
    {
        m_producer = (m_producer + 1) % LOG_QUEUE_SIZE;
    }

private:
    void loggerWorkerThread();

public:
    bool setLogFilepath(std::string filename);
    void setLogKeepDays(int keepDays);

private:
    std::string  m_filepath;
    std::string  m_filename;
    std::fstream m_logInfoFile;
    std::fstream m_logWarnFile;
    std::fstream m_logErrorFile;

private:
    int         m_keepDays;
    int         m_producer;
    int         m_consumer;
    LogMessage *m_logQueue;
};
} // namespace logutils