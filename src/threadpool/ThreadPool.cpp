#include "ThreadPool.h"
#include "ThreadTask.h"
#include <mutex>

ThreadPool::ThreadPool(int n)
{
    m_runFlags = false;
    m_poolSize = n;
    m_threads = new std::thread[n];
}

ThreadPool::~ThreadPool()
{
    if (m_threads) {
        delete[] m_threads;
        m_threads = nullptr;
    }
}

void ThreadPool::thread_main()
{
    while (true) {
        std::unique_lock<std::mutex> lock(m_lock);
        if (m_taskQueue.size() == 0 && m_runFlags) {
            m_cond.wait(lock);
        }

        // 线程池退出
        if (!m_runFlags) {
            break;
        }

        ThreadTask *task = m_taskQueue.front();
        m_taskQueue.pop_front();
        lock.unlock();

        // 运行线程池任务
        task->runTask();
    }
}

void ThreadPool::start()
{
    m_runFlags = true;
    for (int i = 0; i < m_poolSize; i++) {
        m_threads[i] = std::thread(&ThreadPool::thread_main, this);
    }
}

void ThreadPool::stop()
{
    if (!m_threads)
        return;

    for (int i = 0; i < m_poolSize; i++) {
        if (m_threads[i].joinable()) {
            m_threads[i].join();
        }
    }
}