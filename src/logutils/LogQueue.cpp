#include "LogQueue.h"
#include "LogConstants.h"
#include <mutex>

namespace logutils {

LogQueue::LogQueue() : m_pdrLocate(0), m_producer(0), m_consumer(0)
{
    for (int i = 0; i < LOG_QUEUE_SIZE; i++) {
        m_logQueue[i] = new LogMessage();
    }
}

LogQueue::~LogQueue()
{
    for (int i = 0; i < LOG_QUEUE_SIZE; i++) {
        if (m_logQueue[i]) {
            delete m_logQueue[i];
        }
    }
}

LogMessage *LogQueue::getConsumer()
{
    if (m_producer != m_consumer) {
        std::lock_guard<std::mutex> guard(m_queueLock);
        if (m_producer != m_consumer) {
            LogMessage *log = m_logQueue[m_consumer];
            m_logQueue[m_consumer] = nullptr;
            return log;
        }
    }
    return nullptr;
}

void LogQueue::putConsumer(LogMessage *logMessage)
{
    std::lock_guard<std::mutex> guard(m_queueLock);
    m_logQueue[m_consumer] = logMessage;
    m_consumer = (m_consumer + 1) % LOG_QUEUE_SIZE;
}

LogMessage *LogQueue::getProducer()
{
    if (((m_producer + 1) % LOG_QUEUE_SIZE) != m_consumer) {
        std::lock_guard<std::mutex> guard(m_queueLock);
        if (((m_producer + 1) % LOG_QUEUE_SIZE) != m_consumer) {
            LogMessage *log = m_logQueue[m_pdrLocate %= LOG_QUEUE_SIZE];
            m_logQueue[m_pdrLocate++] = nullptr;
            return log;
        }
    }
    return nullptr;
}

void LogQueue::putProducer(LogMessage *logMessage)
{
    std::lock_guard<std::mutex> guard(m_queueLock);
    m_logQueue[m_producer] = logMessage;
    m_producer = (m_producer + 1) % LOG_QUEUE_SIZE;
}

} // namespace logutils
