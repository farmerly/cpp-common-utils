#include "LogManager.h"
#include "FileUtils.h"
#include "LogQueue.h"
#include "StringUtils.h"
#include "TimeUtils.h"
#include <chrono>
#include <cstring>
#include <ctime>
#include <dirent.h>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <thread>

using namespace std;
using namespace logutils;

LogManager *g_logManager = new LogManager();

LogManager::LogManager()
{
    m_bRunning = true;
    m_bWriteLog = false;
    m_logLevel = LOG_LEVEL_INFO;
    m_keepDays = 0;
    m_lastTime = 0;
    m_logQueue = new LogQueue();
    m_thHandle = std::thread(&LogManager::loggerWorkerThread, this);
}

LogManager::~LogManager()
{
    m_bRunning = false;
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
    m_bWriteLog = true;
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
        log = m_logQueue->getProducer();
    } while (log == nullptr);

    std::va_list args;
    va_start(args, format);
    log->setMessage(level, format, args);
    va_end(args);

    m_logQueue->putConsumer(log);
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

bool LogManager::removeLogFiles()
{
    auto checkTime = get_current_timestamp_seconds();
    checkTime = checkTime - m_keepDays * (24 * 60 * 60);
    DIR *dir = opendir(m_filepath.c_str());
    if (dir == nullptr)
        return false;

    dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
            continue;
        }

        std::string filename = std::string(entry->d_name);
        if (entry->d_type != DT_DIR && string_starts_with(filename, m_filename)) {
            int position = filename.rfind('.');

            struct tm timeinfo;
            timeinfo.tm_year = atoi(filename.substr(position + 1, 4).c_str()) - 1900;
            timeinfo.tm_mon = atoi(filename.substr(position + 5, 2).c_str()) - 1;
            timeinfo.tm_mday = atoi(filename.substr(position + 7, 2).c_str());

            if (checkTime > mktime(&timeinfo)) {
                remove_file((m_filepath + filename).c_str());
            }
        }
    }
    return true;
}

void LogManager::loggerWorkerThread()
{
    while (m_bRunning) {
        auto curTime = get_current_timestamp_seconds();
        if (curTime - m_lastTime > 86400) {
            m_lastTime = curTime - curTime % 86400;

            auto iter = m_logStream.begin();
            for (; iter != m_logStream.end(); iter++) {
                if (iter->second.is_open())
                    iter->second.close();
            }

            if (m_keepDays > 0) {
                removeLogFiles();
            }
        }

        LogMessage *log = m_logQueue->getConsumer();
        if (log == nullptr) {
            this_thread::sleep_for(chrono::milliseconds(1));
            continue;
        }

        if (m_bWriteLog && log->level() >= m_logLevel) {
            if (!m_logStream[log->level()].is_open()) {
                openLogFile(log->level());
            }
            m_logStream[log->level()].write(log->getMessage(), log->length());
            m_logStream[log->level()].flush();
        } else {
            printf("%s", log->getMessage());
        }
        m_logQueue->putProducer(log);
    }
}