#pragma once

#include "LogMessage.h"
#include <iomanip>
#include <mutex>

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
    uint32_t    m_pdrLocate;
    uint32_t    m_producer;
    uint32_t    m_consumer;
    std::mutex  m_queueLock;
    LogMessage *m_logQueue[LOG_QUEUE_SIZE];
};
} // namespace logutils