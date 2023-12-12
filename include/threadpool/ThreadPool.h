#ifndef THREAD_POOL_H_H_H
#define THREAD_POOL_H_H_H

#include "ThreadTask.h"
#include <condition_variable>
#include <list>
#include <mutex>
#include <thread>

class ThreadPool
{
public:
    ThreadPool(int n);
    virtual ~ThreadPool();

protected:
    void thread_main();

public:
    void start();
    void stop();

private:
    std::thread            *m_threads;
    int                     m_poolSize;
    bool                    m_runFlags;
    std::list<ThreadTask *> m_taskQueue;
    std::mutex              m_lock;
    std::condition_variable m_cond;
};

#endif