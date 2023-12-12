#include "LogManager.h"
#include "FileUtils.h"
#include <chrono>
#include <cstring>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <thread>

using namespace std;
using namespace logutils;

LogManager *g_logManager = new LogManager();

LogManager::LogManager()
{
    m_pdrTemp = 0;
    m_producer = 0;
    m_consumer = 0;
    m_running = true;
    m_logQueue = new LogMessage[LOG_QUEUE_SIZE];
    m_thHandle = std::thread(&LogManager::loggerWorkerThread, this);
}

LogManager::~LogManager()
{
    m_running = false;
    if (m_thHandle.joinable())
        m_thHandle.join();
    if (m_logQueue)
        delete m_logQueue;
}

bool LogManager::setLogFilepath(std::string filename)
{
    unsigned long pos = filename.rfind('/');
#if defined(_MSC_VER)
    if (pos == std::string::npos) {
        pos = filename.rfind('\\');
    }
#endif
    if (pos == std::string::npos) {
        return false;
    }

    // 递归创建日志目录
    create_directory_recurse(filename.substr(0, pos));
    m_filepath = filename.substr(0, pos + 1);
    m_filename = filename.substr(pos + 1, filename.length());
    return true;
}

void LogManager::setLogKeepDays(int keepDays)
{
    // 设置日志留存期限
    m_keepDays = keepDays;
}

void LogManager::setConsoleLevel(int level)
{
    // 设置日志打印级别
    m_logLevel = level;
}

void LogManager::logRecord(int level, const char *format, ...)
{
    LogMessage *log = nullptr;
    do {
        log = getProducer();
    } while (log == nullptr);

    va_list args;
    va_start(args, format);
#if defined(__GNUC__)
    vsnprintf(log->message, LOG_LINE_SIZE, format, args);
#elif defined(_MSC_VER)
    vsprintf_s(log->message, format, args);
#endif
    va_end(args);

    log->level = level;
    putProducer();
}

LogMessage *LogManager::getConsumer()
{
    return (m_producer != m_consumer) ? &m_logQueue[m_consumer] : nullptr;
}

void LogManager::putConsumer()
{
    m_consumer = (m_consumer + 1) % LOG_QUEUE_SIZE;
}

LogMessage *LogManager::getProducer()
{
    return ((m_producer + 1) % LOG_QUEUE_SIZE) != m_consumer ? &m_logQueue[(m_pdrTemp = (m_pdrTemp + 1) % LOG_QUEUE_SIZE)] : nullptr;
}

void LogManager::putProducer()
{
    m_producer = (m_producer + 1) % LOG_QUEUE_SIZE;
}

void LogManager::openLogFile(int level)
{
    std::ostringstream oss;

    // 格式化时间字符串
    auto   tp = std::chrono::high_resolution_clock::now();
    time_t timer = std::chrono::system_clock::to_time_t(tp);
    oss << m_filepath << m_filename;
    switch (level) {
        case LOG_LEVEL_ERROR:
            oss << "Error.log.";
            break;
        case LOG_LEVEL_WARN:
            oss << "Warn.log.";
            break;
        case LOG_LEVEL_INFO:
        case LOG_LEVEL_DEBUG:
        default:
            oss << "Info.log.";
    }
    oss << std::put_time(std::localtime(&timer), "%Y%m%d");

    std::string logFileName = oss.str();
    m_logStream[level].open(logFileName, ios::in | ios::out | ios::app);
}

void LogManager::loggerWorkerThread()
{
    while (m_running) {
        LogMessage *log = getConsumer();
        if (log == nullptr) {
            this_thread::sleep_for(chrono::milliseconds(1));
            continue;
        }

        if (log->level > LOG_LEVEL_DEBUG) {
            if (!m_logStream[log->level].is_open()) {
                openLogFile(log->level);
            }
            m_logStream[log->level].write(log->message, strlen(log->message));
            m_logStream[log->level].flush();
        }
        if (m_logLevel >= log->level) {
            printf("%s", log->message);
        }
        putConsumer();
    }
}