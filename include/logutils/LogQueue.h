#pragma once

#include "LogMessage.h"
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
    LogMessage *getConsumer();
    void        putConsumer(LogMessage *logMessage);
    LogMessage *getProducer();
    void        putProducer(LogMessage *logMessage);

private:
    std::deque<LogMessage *> m_producerQueue;
    std::deque<LogMessage *> m_consumerQueue;
    std::mutex               m_producerLock;
    std::mutex               m_consumerLock;
};

} // namespace logutils