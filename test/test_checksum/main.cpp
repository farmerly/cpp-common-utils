#include "Common.h"
#include "crc16.h"
#include <arpa/inet.h>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    unsigned char data[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
    uint16_t      code = checksum::crc16_xmodem(data, sizeof(data));
    unsigned char checksum_buf[2] = {0};
    *(uint16_t *)checksum_buf = htons(code);

    std::cout << "data:  " << get_hex_string(data, sizeof(data)) << std::endl;
    std::cout << "crc16: " << get_hex_string(checksum_buf, sizeof(checksum_buf)) << std::endl;
    return 0;
}