#include "Logging.h"
#include "TimeUtils.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

using namespace std;

int main(int argc, char *argv[])
{
    std::atomic<uint32_t> value(1);
    init_log_manager("/root/logs/log");

    int         threadnum = 10;
    std::thread threads[threadnum];
    auto        startTime = get_current_timestamp_millis();
    for (int i = 0; i < threadnum; i++) {
        threads[i] = std::thread([&]() {
            thread::id    tid = this_thread::get_id();
            ostringstream oss;
            oss << tid;

            for (int i = 0; i < 1000; i++) {
                auto tmp = value.fetch_add(1);
                log_debug("%s, hello, world, %d", oss.str().c_str(), tmp);
                log_info("%s, hello, world, %d", oss.str().c_str(), tmp);
                log_warn("%s, hello, world, %d", oss.str().c_str(), tmp);
                log_error("%s, hello, world, %d", oss.str().c_str(), tmp);
            }
        });
    }
    for (int i = 0; i < threadnum; i++) {
        threads[i].join();
    }
    auto endTime = get_current_timestamp_millis();
    std::cout << "cost ms: " << endTime - startTime << std::endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    return 0;
}