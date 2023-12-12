// =====================================================================================
//	Copyright (C) 2019 by Jiaxing Shao.All rights reserved.
//
//	文 件 名:  base64.h
//	作    者:  Jiaxing Shao, 13315567369@163.com
//	版 本 号:  1.0
//	创建时间:  2019年06月19日 01时59分55秒
//	Compiler:  g++
//	描    述:
// =====================================================================================

#ifndef _BASE64_ENCRYPT_H_H_H
#define _BASE64_ENCRYPT_H_H_H

#include <cstring>
#include <sstream>
#include <string>

namespace crypto {

/**
 * base64编码
 * @param srcBuf 需要进行 base64 编码的原始数据
 * @param srcLen 原始数据长度
 * @param dstBuf 编码完成后的数据
 * @param dstMaxLen dstBuf缓存最大长度
 */
int base64_encode(const char *srcBuf, int srcLen, char *dstBuf, int dstMaxLen);

/**
 * base64解码
 * @param srcBuf 需要进行 base64 解码的原始数据
 * @param srcLen 原始数据长度
 * @param dstBuf 解码完成后的数据
 * @param dstMaxLen dstBuf缓存最大长度
 */
int base64_decode(const char *srcBuf, int srcLen, char *dstBuf, int dstMaxLen);

} // namespace crypto
#endif
