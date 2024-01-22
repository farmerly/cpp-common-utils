#include "LogQueue.h"
#include "LogConstants.h"
#include "LogMessage.h"
#include <chrono>
#include <mutex>

using namespace std::chrono;

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

LogMessage *LogQueue::getConsumer(uint32_t waitMs)
{
    if (m_consumerQueue.size() == 0 && waitMs == 0) {
        return nullptr;
    }

    std::unique_lock<std::mutex> lock(m_consumerLock);
    if (m_consumerQueue.size() == 0 && waitMs != 0) {
        m_consumerCond.wait_for(lock, milliseconds(waitMs));
    }
    if (m_consumerQueue.size() > 0) {
        LogMessage *log = m_consumerQueue.front();
        m_consumerQueue.pop_front();
        return log;
    }
    return nullptr;
}

void LogQueue::putConsumer(LogMessage *logMessage)
{
    std::lock_guard<std::mutex> guard(m_consumerLock);
    m_consumerQueue.push_back(logMessage);
    m_consumerCond.notify_one();
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

size_t LogQueue::getConsumerSize()
{
    return m_consumerQueue.size();
}

size_t LogQueue::getProducerSize()
{
    return m_producerQueue.size();
}

} // namespace logutils
