#include "LogManager.h"
#include "FileUtils.h"

using namespace logutils;

LogManager *g_logManager = new LogManager();

LogManager::LogManager()
{
    m_logQueue = new LogMessage[LOG_QUEUE_SIZE];
}

LogManager::~LogManager()
{
    if (m_logQueue)
        delete m_logQueue;
}

void LogManager::loggerWorkerThread()
{
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

    create_directory_recurse(filename.substr(0, pos));
    m_filepath = filename.substr(0, pos + 1);
    m_filename = filename.substr(pos + 1, filename.length());
    return true;
}

void LogManager::setLogKeepDays(int keepDays)
{
    m_keepDays = keepDays;
}