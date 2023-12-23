#include "bbc.h"

unsigned char bcc_check_8(unsigned char *buf, unsigned int len)
{
    unsigned char bcc = *buf;
    for (unsigned int i = 1; i < len; i++) {
        bcc ^= buf[i];
    }
    return bcc;
}