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
    std::thread            *m_threads;   // 线程队列
    int                     m_poolSize;  // 线程池大小
    bool                    m_runFlags;  // 运行标识
    std::list<ThreadTask *> m_taskQueue; // 任务队列
    std::mutex              m_lock;
    std::condition_variable m_cond;
};

#endif