// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  md5.h
//  作    者:  Jiaxing Shao, 13315567369@163.com
//  版 本 号:  1.0
//  创建时间:  2020年01月14日
//  Compiler:  g++
//  描    述:
// =====================================================================================

#ifndef _MD5_ENCRYPT_H_H_H
#define _MD5_ENCRYPT_H_H_H

#include <cstring>
#include <iomanip>
#include <stdlib.h>
#include <string>

namespace crypto {

/**
 * @param srcBuf 待加密数据
 * @param srcLen 待加密数据长度
 * @return 加密后的 hex 字符串
 */
std::string md5_encrypt_hex(unsigned char *srcBuf, int srcLen);

} // namespace crypto

#endif
