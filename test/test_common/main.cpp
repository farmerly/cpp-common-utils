#include "Common.h"
#include "StringUtils.h"
#include "TimeUtils.h"
#include <ctime>
#include <iostream>

using namespace std;

void test_timeutils();
void test_strutils();

int main(int argc, char *argv[])
{
    // test_timeutils();
    test_strutils();
    return 0;
}

void test_timeutils()
{
    std::string datetime = "2014-03-14T15:13:00.885";
    uint64_t    timestamp = datetime_to_timestamp(datetime);
    std::cout << "timestamp: " << timestamp << std::endl;
}

void test_strutils()
{
    std::string message = ";RTP/AVP;multicast;;ttl=127;";
    auto        vec = split_strings(message, ";");
    for (int i = 0; i < vec.size(); i++) {
        std::cout << vec[i] << std::endl;
    }
}