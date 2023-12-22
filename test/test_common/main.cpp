#include "Common.h"
#include "Configuration.h"
#include "StringUtils.h"
#include "TimeUtils.h"
#include <ctime>
#include <iomanip>
#include <iostream>

using namespace std;

void test_configure();
void test_timeutils();
void test_strutils();

int main(int argc, char *argv[])
{
    test_configure();
    // test_timeutils();
    // test_strutils();
    return 0;
}

void test_configure()
{
    std::string str = get_config_prop_string("./configure.ini", "string", "message");
    int         i = get_config_prop_int("./configure.ini", "int", 65535);
    double      d = get_config_prop_double("./configure.ini", "double", 1.123456789);

    std::cout << "string: " << str << std::endl;
    std::cout << "int:    " << i << std::endl;
    std::cout << "double: " << setprecision(10) << d << std::endl;
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