#include "MapValue.h"
#include <cstring>
#include <iostream>
#include <ostream>

int main(int argc, char *argv[])
{
    {
        MapValue value;
        value["a"] = "abc";
        value["b"] = 1;
        value["c"] = 0.1;
        value["d"] = false;
        std::cout << value["a"].asString() << std::endl;
        std::cout << value["b"].asInt() << std::endl;
        std::cout << value["c"].asDouble() << std::endl;
        std::cout << value["d"].asBool() << std::endl;
    }

    {
        MapValue value;
        MapValue arr1("1");
        MapValue arr2("2");
        value.append(arr1);
        value.append(arr2);
        std::cout << value[0].asString() << std::endl;
        std::cout << value[1].asString() << std::endl;
    }
    return 0;
}