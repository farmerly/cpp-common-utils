#include "Logger.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

using namespace std;

int main(int argc, char *argv[])
{
    init_log_manager("/root/logs/log");

    std::thread threads[10];
    for (int i = 0; i < 10; i++) {
        threads[i] = std::thread([]() {
            thread::id    tid = this_thread::get_id();
            ostringstream oss;
            oss << tid;

            std::cout << "开始 " << tid << std::endl;
            for (int i = 0; i < 100; i++) {
                log_info("%s, hello, world, %d", oss.str().c_str(), i + 1);
            }
            std::cout << "结束 " << tid << std::endl;
        });
    }
    for (int i = 0; i < 10; i++) {
        threads[i].join();
    }
    this_thread::sleep_for(chrono::milliseconds(3000));
}