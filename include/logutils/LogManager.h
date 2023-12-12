#pragma once

#include <cstdarg>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <thread>
namespace logutils {

static constexpr int LOG_LEVEL_DEBUG = 0;
static constexpr int LOG_LEVEL_INFO = 1;
static constexpr int LOG_LEVEL_WARN = 2;
static constexpr int LOG_LEVEL_ERROR = 3;

static constexpr int LOG_LINE_SIZE = 1024 * 16;  // 单行日志大小
static constexpr int LOG_QUEUE_SIZE = 1024 * 16; // Log 队列长度

struct LogMessage {
    int  level;
    char message[LOG_LINE_SIZE];
};

class LogManager
{
public:
    LogManager();
    virtual ~LogManager();

public:
    bool setLogFilepath(std::string filename);
    void setLogKeepDays(int keepDays);
    void setConsoleLevel(int level);
    void logRecord(int level, const char *format, ...);

private:
    inline LogMessage *getConsumer();
    inline void        putConsumer();
    inline LogMessage *getProducer();
    inline void        putProducer();

private:
    void openLogFile(int level);
    void loggerWorkerThread();

private:
    std::string                 m_filepath;
    std::string                 m_filename;
    std::map<int, std::fstream> m_logStream;

private:
    bool        m_running;
    int         m_logLevel;
    int         m_keepDays;
    int         m_pdrTemp;
    int         m_producer;
    int         m_consumer;
    LogMessage *m_logQueue;
    std::thread m_thHandle;
};
} // namespace logutils