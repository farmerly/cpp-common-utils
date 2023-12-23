#ifndef BBC_H_H_H
#define BBC_H_H_H

namespace checksum {
/**
 * 异或校验
 * @param buf 要校验的数据
 * @param len 数据长度
 * @return 校验值
 */
unsigned char bcc_check_8(unsigned char *buf, unsigned int len);
} // namespace checksum

#endif