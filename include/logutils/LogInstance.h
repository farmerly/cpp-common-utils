#pragma once

#include <mutex>

namespace logutils {

template <typename T>
class LogInstance
{
protected:
    LogInstance() = default;
    LogInstance(const LogInstance &) = default;
    virtual ~LogInstance() = default;

protected:
    LogInstance &operator=(const LogInstance &){};

public:
    static T *getInstance()
    {
        if (m_instance == nullptr) {
            std::lock_guard<std::mutex> guard(m_mutex);
            if (m_instance == nullptr) {
                m_instance = new T();
            }
        }
        return m_instance;
    }

private:
    static std::mutex m_mutex;
    static T         *m_instance;
};

template <typename T>
std::mutex LogInstance<T>::m_mutex;

template <typename T>
T *LogInstance<T>::m_instance = nullptr;
} // namespace logutils