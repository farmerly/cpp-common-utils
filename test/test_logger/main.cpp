#include "Logging.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

using namespace std;

int main(int argc, char *argv[])
{
    std::atomic<uint32_t> value(1);
    init_log_manager("/root/logs/log");

    std::thread threads[10];
    for (int i = 0; i < 10; i++) {
        threads[i] = std::thread([&]() {
            thread::id    tid = this_thread::get_id();
            ostringstream oss;
            oss << tid;

            for (int i = 0; i < 10000; i++) {
                auto tmp = value.fetch_add(1);
                log_info("%s, hello, world, %d", oss.str().c_str(), tmp);
            }
        });
    }
    for (int i = 0; i < 10; i++) {
        threads[i].join();
    }
    this_thread::sleep_for(chrono::milliseconds(1000));
}