// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  des.h
//  作    者:  Jiaxing Shao, 13315567369@163.com
//  版 本 号:  1.0
//  创建时间:  2020年01月14日
//  Compiler:  g++
//  描    述:
// =====================================================================================

#ifndef _DES_ENCRYPT_H_H_H
#define _DES_ENCRYPT_H_H_H

#include "base64.h"
#include <cstring>
#include <iomanip>
#include <math.h>
#include <string>

namespace crypto {

enum PaddingType {
    ZeroPadding = 0,
    Pkcs5Padding = 1,
    Pkcs7Padding = 2
};

typedef struct {
    bool SubKey[3][16][48];
    bool Is3DES;
} SecretKey;

/** get secret key */
SecretKey *GetSecretKey(const char *pKey, int nLen, bool is3Des);
int        DesEncrypt(SecretKey *secretKey, const char *srcBuf, int srcLen, char *dstBuf, int dstMaxLen, PaddingType padType);
int        DesDecrypt(SecretKey *secretKey, const char *srcBuf, int srcLen, char *dstBuf, int dstMaxLen, PaddingType padType);

std::string DesEncryptBase64(SecretKey *secretKey, const char *pBuf, int nLen, PaddingType padType);
int         DesDecryptBase64(SecretKey *secretKey, std::string &strMsg, char *pBuf, int nMaxLen, PaddingType padType);

} // namespace crypto
#endif
