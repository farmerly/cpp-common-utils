// =====================================================================================
//	Copyright (C) 2019 by Jiaxing Shao.All rights reserved.
//
//	文 件 名:  base64.cpp
//	作    者:  Jiaxing Shao, 980853650@qq.com
//	版 本 号:  1.0
//	创建时间:  2019年06月19日 02时00分01秒
//	Compiler:  g++
//	描    述:
// =====================================================================================

#include "base64.h"

namespace kgr {
    namespace crypto {

        int base64_encode(char *dstBuf, int dstMaxLen, const char *srcBuf, int srcLen)
        {
            static constexpr unsigned char BASE64_MAP_ASC[64] = {
                'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
                'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

            if (srcLen <= 0 || ((srcLen + 2) / 3) * 4 > dstMaxLen)
                return -1;

            int dstLen = 0;
            for (int i = 0; i < srcLen; i += 3) {
                if (i + 1 == srcLen) {
                    dstBuf[dstLen++] = BASE64_MAP_ASC[(srcBuf[i] & 0xFC) >> 2];
                    dstBuf[dstLen++] = BASE64_MAP_ASC[(srcBuf[i] & 0x03) << 4];
                    dstBuf[dstLen++] = '=';
                    dstBuf[dstLen++] = '=';
                } else if (i + 2 == srcLen) {
                    dstBuf[dstLen++] = BASE64_MAP_ASC[(srcBuf[i] & 0xFC) >> 2];
                    dstBuf[dstLen++] = BASE64_MAP_ASC[((srcBuf[i] & 0x03) << 4) | ((srcBuf[i + 1] & 0xF0) >> 4)];
                    dstBuf[dstLen++] = BASE64_MAP_ASC[(srcBuf[i + 1] & 0x0F) << 2];
                    dstBuf[dstLen++] = '=';
                } else {
                    dstBuf[dstLen++] = BASE64_MAP_ASC[(srcBuf[i] & 0xFC) >> 2];
                    dstBuf[dstLen++] = BASE64_MAP_ASC[((srcBuf[i] & 0x03) << 4) | ((srcBuf[i + 1] & 0xF0) >> 4)];
                    dstBuf[dstLen++] = BASE64_MAP_ASC[((srcBuf[i + 1] & 0x0F) << 2) | ((srcBuf[i + 2] & 0xC0) >> 6)];
                    dstBuf[dstLen++] = BASE64_MAP_ASC[srcBuf[i + 2] & 0x3F];
                }
            }
            return dstLen;
        }

        int base64_decode(char *dstBuf, int dstMaxLen, const char *srcBuf, int srcLen)
        {
            static constexpr unsigned char BASE64_MAP_DESC[256] = {
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x3F, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
                0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1A, 0x1B, 0x1C,
                0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
                0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

            if (srcLen % 4 != 0)
                return -1;

            int dstLen = 0;
            for (int i = 0; i < srcLen; i += 4) {
                dstBuf[dstLen] = (BASE64_MAP_DESC[(unsigned char)srcBuf[i]] & 0x3F) << 2;
                dstBuf[dstLen++] |= (BASE64_MAP_DESC[(unsigned char)srcBuf[i + 1]] & 0x30) >> 4;

                dstBuf[dstLen] = (BASE64_MAP_DESC[(unsigned char)srcBuf[i + 1]] & 0x0F) << 4;
                dstBuf[dstLen++] |= (BASE64_MAP_DESC[(unsigned char)srcBuf[i + 2]] & 0x3C) >> 2;

                dstBuf[dstLen] = (BASE64_MAP_DESC[(unsigned char)srcBuf[i + 2]] & 0x03) << 6;
                dstBuf[dstLen++] |= (BASE64_MAP_DESC[(unsigned char)srcBuf[i + 3]] & 0x3F);
            }

            for (int i = srcLen - 1; i > 0; i--) {
                if (srcBuf[i] == '=') {
                    dstLen -= 1;
                } else {
                    break;
                }
            }
            return dstLen;
        }
    } // namespace crypto
} // namespace kgr