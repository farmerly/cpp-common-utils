// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  des.cpp
//  作    者:  Jiaxing Shao, 13315567369@163.com
//  版 本 号:  1.0
//  创建时间:  2020年01月14日
//  Compiler:  g++
//  描    述:
// =====================================================================================

#include "des.h"

using namespace crypto;

static const int KEY_TABLE[56] = {57, 49, 41, 33, 25, 17, 9,  1,  58, 50, 42, 34, 26, 18, 10, 2,  59, 51, 43,
                                  35, 27, 19, 11, 3,  60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7,  62, 54,
                                  46, 38, 30, 22, 14, 6,  61, 53, 45, 37, 29, 21, 13, 5,  28, 20, 12, 4};

static const int KEY_SHIFT[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

static const int COMPRESS_TABLE[48] = {14, 17, 11, 24, 1,  5,  3,  28, 15, 6,  21, 10, 23, 19, 12, 4,  26, 8,  16, 7,  27, 20, 13, 2,
                                       41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};

static const int IP_TABLE[64] = {58, 50, 42, 34, 26, 18, 10, 2,  60, 52, 44, 36, 28, 20, 12, 4,  62, 54, 46, 38, 30, 22,
                                 14, 6,  64, 56, 48, 40, 32, 24, 16, 8,  57, 49, 41, 33, 25, 17, 9,  1,  59, 51, 43, 35,
                                 27, 19, 11, 3,  61, 53, 45, 37, 29, 21, 13, 5,  63, 55, 47, 39, 31, 23, 15, 7};

const int EXTEND_TABLE[48] = {32, 1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,  8,  9,  10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
                              16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1};

static const int S_BOX_TABLE[8][4][16] = {
    {
     {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
     {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
     {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
     {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},
     },
    {
     {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
     {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
     {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
     {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},
     },
    {
     {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
     {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
     {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
     {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},
     },
    {
     {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
     {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
     {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
     {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},
     },
    {
     {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
     {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
     {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
     {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},
     },
    {
     {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
     {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
     {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
     {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},
     },
    {
     {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
     {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
     {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
     {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},
     },
    {
     {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
     {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
     {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
     {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11},
     }
};

static const int P_TABLE[32] = {16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23, 26, 5,  18, 31, 10,
                                2,  8, 24, 14, 32, 27, 3,  9,  19, 13, 30, 6,  22, 11, 4,  25};

static const int IPEND_TABLE[64] = {40, 8,  48, 16, 56, 24, 64, 32, 39, 7,  47, 15, 55, 23, 63, 31, 38, 6,  46, 14, 54, 22,
                                    62, 30, 37, 5,  45, 13, 53, 21, 61, 29, 36, 4,  44, 12, 52, 20, 60, 28, 35, 3,  43, 11,
                                    51, 19, 59, 27, 34, 2,  42, 10, 50, 18, 58, 26, 33, 1,  41, 9,  49, 17, 57, 25};

void ByteToBinary(char ch, bool binary[8])
{
    for (int i = 0; i < 8; i++) {
        binary[i] = (ch >> (7 - i)) & 0x01;
    }
}

void KeyLeftShift(bool key56[56], unsigned int round)
{
    bool L28[28] = {0};
    bool R28[28] = {0};
    for (int i = 0; i < 28; i++) {
        L28[i] = key56[i];
        R28[i] = key56[i + 28];
    }

    for (int i = 0; i < 28; i++) {
        if ((i + KEY_SHIFT[round]) < 28) {
            key56[i] = L28[i + KEY_SHIFT[round]];
            key56[i + 28] = R28[i + KEY_SHIFT[round]];
        } else {
            key56[i] = L28[i + KEY_SHIFT[round] - 28];
            key56[i + 28] = R28[i + KEY_SHIFT[round] - 28];
        }
    }
}

void CreateSubKey(SecretKey *secretKey, char *pKey, int keyN)
{
    bool dec64[64] = {0};
    bool key64[64] = {0};
    bool key56[56] = {0};
    for (int i = 0; i < 8; i++) {
        ByteToBinary(pKey[i], dec64 + i * 8);
    }
    for (int i = 0; i < 64; i++) {
        key64[i] = dec64[63 - i];
    }
    for (int i = 0; i < 56; i++) {
        key56[i] = key64[64 - KEY_TABLE[i]];
    }
    for (int i = 0; i < 16; i++) {
        KeyLeftShift(key56, i);
        for (int j = 0; j < 48; j++) {
            secretKey->SubKey[keyN][i][j] = key56[COMPRESS_TABLE[j] - 1];
        }
    }
}

void F_Function(bool r32[32], bool subKey[48], bool out32[32])
{
    bool expandR[48] = {0};
    for (int i = 0; i < 48; i++) {
        expandR[i] = r32[EXTEND_TABLE[i] - 1];
        expandR[i] = expandR[i] ^ subKey[i];
    }
    bool t32[32] = {0};
    for (int i = 0; i < 8; i++) {
        int row = expandR[i * 6 + 0] * 2 + expandR[i * 6 + 5];
        int col = expandR[i * 6 + 4] + expandR[i * 6 + 3] * 2 + expandR[i * 6 + 2] * 4 + expandR[i * 6 + 1] * 8;
        int num = S_BOX_TABLE[i][row][col];
        t32[i * 4 + 0] = num & 0x08 ? true : false;
        t32[i * 4 + 1] = num & 0x04 ? true : false;
        t32[i * 4 + 2] = num & 0x02 ? true : false;
        t32[i * 4 + 3] = num & 0x01 ? true : false;
    }

    for (int i = 0; i < 32; i++) {
        out32[i] = t32[P_TABLE[i] - 1];
    }
}

void DESEncode64(SecretKey *secretKey, const char *pBuf, char *pDst, int keyN)
{
    bool dec64[64] = {0};
    bool tmp64[64] = {0};
    for (int i = 0; i < 8; i++) {
        ByteToBinary(pBuf[i], dec64 + i * 8);
    }
    for (int i = 0; i < 64; i++) {
        tmp64[i] = dec64[63 - i];
    }
    bool T64[64] = {0};
    for (int i = 0; i < 64; i++) {
        T64[i] = tmp64[64 - IP_TABLE[i]];
    }
    bool L32[32] = {0};
    bool R32[32] = {0};
    for (int i = 0; i < 32; i++) {
        L32[i] = T64[i];
        R32[i] = T64[i + 32];
    }
    for (int i = 0; i < 16; i++) {
        bool T32[32] = {0};
        F_Function(R32, secretKey->SubKey[keyN][i], T32);
        for (int j = 0; j < 32; j++) {
            T32[j] = L32[j] ^ T32[j];
            L32[j] = R32[j];
            R32[j] = T32[j];
        }
    }
    for (int i = 0; i < 32; i++) {
        T64[i] = R32[i];
        T64[i + 32] = L32[i];
    }
    bool bit64[64] = {0};
    for (int i = 0; i < 64; i++) {
        bit64[i] = T64[IPEND_TABLE[i] - 1];
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (bit64[i * 8 + j]) {
                pDst[i] |= ((int)pow(2, (7 - j)) & 0xFF);
            }
        }
    }
}

void DESDecode64(SecretKey *secretKey, const char *pBuf, char *pDst, int keyN)
{
    bool dec64[64] = {0};
    bool tmp64[64] = {0};
    for (int i = 0; i < 8; i++) {
        ByteToBinary(pBuf[i], dec64 + i * 8);
    }
    for (int i = 0; i < 64; i++) {
        tmp64[i] = dec64[63 - i];
    }
    bool T64[64] = {0};
    for (int i = 0; i < 64; i++) {
        T64[i] = tmp64[64 - IP_TABLE[i]];
    }
    bool L32[32] = {0};
    bool R32[32] = {0};
    for (int i = 0; i < 32; i++) {
        L32[i] = T64[i + 32];
        R32[i] = T64[i];
    }
    for (int i = 0; i < 16; i++) {
        bool T32[32] = {0};
        F_Function(L32, secretKey->SubKey[keyN][15 - i], T32);
        for (int j = 0; j < 32; j++) {
            T32[j] = R32[j] ^ T32[j];
            R32[j] = L32[j];
            L32[j] = T32[j];
        }
    }
    for (int i = 0; i < 32; i++) {
        T64[i] = L32[i];
        T64[i + 32] = R32[i];
    }
    bool bit64[64] = {0};
    for (int i = 0; i < 64; i++) {
        bit64[i] = T64[IPEND_TABLE[i] - 1];
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (bit64[i * 8 + j]) {
                pDst[i] |= ((int)pow(2, (7 - j)) & 0xFF);
            }
        }
    }
}

unsigned int DESEncode(SecretKey *secretKey, const char *pBuf, unsigned int nLen, char *pDst, int keyN)
{
    int nIndex = 0;
    int srcLen = nLen;
    int dstLen = 0;
    memset(pDst, 0, nLen);

    do {
        DESEncode64(secretKey, pBuf + nIndex, pDst + dstLen, keyN);
        nIndex += 8;
        srcLen -= 8;
        dstLen += 8;
    } while (srcLen > 0);
    return dstLen;
}

/* *
 *
 * */
unsigned int DESDecode(SecretKey *secretKey, const char *pBuf, unsigned int nLen, char *pDst, int keyN)
{
    int nIndex = 0;
    int srcLen = nLen;
    int dstLen = 0;
    memset(pDst, 0, nLen);

    do {
        DESDecode64(secretKey, pBuf + nIndex, pDst + dstLen, keyN);
        nIndex += 8;
        srcLen -= 8;
        dstLen += 8;
    } while (srcLen > 0);
    return dstLen;
}

SecretKey *GetSecretKey(const char *pKey, int nLen, bool is3Des)
{
    SecretKey *secretKey = new SecretKey();
    char       keyBuf[24] = {0};
    int        keyLen = nLen >= 24 ? 24 : nLen;
    memcpy(keyBuf, pKey, keyLen);
    if (is3Des) {
        secretKey->Is3DES = true;
        for (int i = 0; i < 3; i++) {
            CreateSubKey(secretKey, keyBuf + i * 8, i);
        }
    } else {
        secretKey->Is3DES = false;
        CreateSubKey(secretKey, keyBuf, 0);
    }
    return secretKey;
}

int DesEncrypt(crypto::SecretKey *secretKey, const char *srcBuf, int srcLen, char *dstBuf, int dstMaxLen, crypto::PaddingType padType)
{
    if (secretKey == NULL) {
        return -1;
    }
    char *tmpBuf = NULL;
    int   tmpLen = 0;
    int   dstLen = 0;

    memset(dstBuf, 0, dstMaxLen);
    if (padType == ZeroPadding) {
        tmpLen = srcLen % 8 == 0 ? srcLen + 8 : (srcLen / 8 + 1) * 8;
        tmpBuf = (char *)malloc(tmpLen);
        memset(tmpBuf, 0, tmpLen);
        memcpy(tmpBuf, srcBuf, srcLen);
    } else if (padType == Pkcs5Padding) {
        return 0;
    } else if (padType == Pkcs7Padding) {
        return 0;
    }

    if (secretKey->Is3DES) {
        dstLen = DESEncode(secretKey, tmpBuf, tmpLen, dstBuf, 0);
        tmpLen = DESDecode(secretKey, dstBuf, dstLen, tmpBuf, 1);
        dstLen = DESEncode(secretKey, tmpBuf, tmpLen, dstBuf, 2);
    } else {
        dstLen = DESEncode(secretKey, tmpBuf, tmpLen, dstBuf, 0);
    }

    if (tmpBuf != NULL) {
        free(tmpBuf);
        tmpBuf = NULL;
    }
    return dstLen;
}

int DesDecrypt(SecretKey *secretKey, const char *srcBuf, int srcLen, char *dstBuf, int dstMaxLen, PaddingType padType)
{
    if (secretKey == NULL || srcLen % 8 != 0) {
        return -1;
    }
    char *tmpBuf = NULL;
    int   tmpLen = 0;
    int   dstLen = 0;

    memset(dstBuf, 0, dstMaxLen);
    tmpBuf = (char *)malloc(srcLen);
    memset(tmpBuf, 0, srcLen);
    memcpy(tmpBuf, srcBuf, srcLen);
    tmpLen = srcLen;
    if (secretKey->Is3DES) {
        dstLen = DESDecode(secretKey, tmpBuf, tmpLen, dstBuf, 2);
        tmpLen = DESEncode(secretKey, dstBuf, dstLen, tmpBuf, 1);
        dstLen = DESDecode(secretKey, tmpBuf, tmpLen, dstBuf, 0);
    } else {
        dstLen = DESDecode(secretKey, tmpBuf, tmpLen, dstBuf, 0);
    }

    if (tmpBuf != NULL) {
        free(tmpBuf);
        tmpBuf = NULL;
    }

    if (padType == ZeroPadding) {
        int offset = 0;
        for (int i = 0; i < 8; i++) {
            if (dstBuf[dstLen - 1 - i] == 0x00) {
                offset += 1;
            } else {
                break;
            }
        }
        dstLen -= offset;
    } else if (padType == Pkcs5Padding) {
    } else if (padType == Pkcs7Padding) {
    }
    return dstLen;
}

std::string DesEncryptBase64(SecretKey *secretKey, const char *pBuf, int nLen, PaddingType padType)
{
    std::string strMsg = "";
    char       *dstBuf = NULL;
    int         dstMaxLen = nLen + 9;
    dstBuf = (char *)malloc(dstMaxLen);
    memset(dstBuf, 0, dstMaxLen);
    int dstLen = crypto::DesEncrypt(secretKey, pBuf, nLen, dstBuf, dstMaxLen, padType);
    do {
        if (dstLen == -1) {
            break;
        }
        char *tmpBuf = (char *)malloc(dstLen * 2);
        memset(tmpBuf, 0, dstLen * 2);
        if (base64_encode(dstBuf, dstLen, tmpBuf, dstLen * 2) != -1) {
            strMsg = tmpBuf;
        }
        if (tmpBuf != NULL) {
            free(tmpBuf);
            tmpBuf = NULL;
        }
    } while (0);
    return strMsg;
}

int DesDecryptBase64(SecretKey *secretKey, std::string &strMsg, char *pBuf, int nMaxLen, PaddingType padType)
{
    int   tmpMaxLen = strMsg.length();
    char *tmpBuf = (char *)malloc(tmpMaxLen);
    int   tmpLen = 0;
    int   dstLen = 0;

    memset(tmpBuf, 0, tmpMaxLen);
    tmpLen = base64_decode(strMsg.c_str(), strMsg.length(), tmpBuf, tmpMaxLen);
    if (tmpLen == -1) {
        return -1;
    }
    dstLen = crypto::DesDecrypt(secretKey, tmpBuf, tmpLen, pBuf, nMaxLen, padType);
    if (tmpBuf != NULL) {
        free(tmpBuf);
        tmpBuf = NULL;
    }
    return dstLen;
}
