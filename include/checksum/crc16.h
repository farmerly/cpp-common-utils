#ifndef CRC16_H_H_H
#define CRC16_H_H_H

namespace checksum {

/**
 * crc16 校验
 * @param data 需校验数据
 * @param len 数据长度
 */
unsigned short crc16_xmodem(unsigned char *data, unsigned short len);

} // namespace checksum
#endif
