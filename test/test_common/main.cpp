#include "Common.h"
#include "TimeUtils.h"
#include <ctime>
#include <iostream>

using namespace std;

void test_timeutils();

int main(int argc, char *argv[])
{
    test_timeutils();
    return 0;
}

void test_timeutils()
{
    std::string datetime = "2014-03-14T15:13:00.885";
    uint64_t    timestamp = datetime_to_timestamp(datetime);
    std::cout << "timestamp: " << timestamp << std::endl;
}