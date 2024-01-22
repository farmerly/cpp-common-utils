#pragma once

#include "LogMessage.h"
#include <condition_variable>
#include <cstddef>
#include <iomanip>
#include <mutex>
#include <queue>

namespace logutils {

class LogQueue
{
public:
    LogQueue();
    virtual ~LogQueue();

public:
    LogMessage *getConsumer(uint32_t waitMs);
    void        putConsumer(LogMessage *logMessage);
    LogMessage *getProducer();
    void        putProducer(LogMessage *logMessage);
    size_t      getConsumerSize();
    size_t      getProducerSize();

private:
    std::deque<LogMessage *> m_producerQueue;
    std::deque<LogMessage *> m_consumerQueue;
    std::mutex               m_producerLock;
    std::mutex               m_consumerLock;
    std::condition_variable  m_consumerCond;
};

} // namespace logutils