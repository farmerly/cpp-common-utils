#include "LogQueue.h"
#include "LogConstants.h"
#include "LogMessage.h"
#include <mutex>

namespace logutils {

LogQueue::LogQueue()
{
    for (int i = 0; i < LOG_QUEUE_SIZE; i++) {
        m_producerQueue.push_back(new LogMessage());
    }
}

LogQueue::~LogQueue()
{
    while (m_producerQueue.size() > 0) {
        LogMessage *log = m_producerQueue.front();
        m_producerQueue.pop_front();
        delete log;
    }

    while (m_consumerQueue.size() > 0) {
        LogMessage *log = m_consumerQueue.front();
        m_consumerQueue.pop_front();
        delete log;
    }
}

LogMessage *LogQueue::getConsumer()
{
    if (m_consumerQueue.size() > 0) {
        std::lock_guard<std::mutex> guard(m_consumerLock);
        if (m_consumerQueue.size() > 0) {
            LogMessage *log = m_consumerQueue.front();
            m_consumerQueue.pop_front();
            return log;
        }
    }
    return nullptr;
}

void LogQueue::putConsumer(LogMessage *logMessage)
{
    std::lock_guard<std::mutex> guard(m_consumerLock);
    m_consumerQueue.push_back(logMessage);
}

LogMessage *LogQueue::getProducer()
{
    if (m_producerQueue.size() > 0) {
        std::lock_guard<std::mutex> guard(m_producerLock);
        if (m_producerQueue.size() > 0) {
            LogMessage *log = m_producerQueue.front();
            m_producerQueue.pop_front();
            return log;
        }
    }
    return nullptr;
}

void LogQueue::putProducer(LogMessage *logMessage)
{
    std::lock_guard<std::mutex> guard(m_producerLock);
    m_producerQueue.push_back(logMessage);
}

} // namespace logutils
