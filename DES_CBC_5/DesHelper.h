#pragma once
#include <string>
#include "cipher_params.h"

typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

namespace GL
{
// Encrypt the message contained in input file with iv and key using DES in CBC mode
void DES_cbc_encrypt(const std::string& inputFile, const std::string& outputFile, uint64_t(&subkeys)[ITERATIONS], uint64_t &iv);
// Decrypt the message contained in input file with iv and key using DES in CBC mode
void DES_cbc_decrypt(const std::string& inputFile, const std::string& outputFile, uint64_t(&subkeys)[ITERATIONS], uint64_t &iv);

// Encrypt the message contained in input file with iv and key using DES in CBC mode
void DES_cbc_encrypt(const char *in, int inLen, char* out, int& outLen, uint64_t(&subkeys)[ITERATIONS], uint64_t &iv);
// Decrypt the message contained in input file with iv and key using DES in CBC mode
void DES_cbc_decrypt(const char *in, int length, char* out, int& outLen, uint64_t(&subkeys)[ITERATIONS], uint64_t &iv);

// Encrypt the message contained in input file with iv and key using DES in CBC mode
//   keyLen ivLen <=8
void DES_cbc_encrypt(const char *in, int inLen, char* out, int& outLen, const char* keydata, int keyLen, const char* ivdata, int ivLen);
// Decrypt the message contained in input file with iv and key using DES in CBC mode
//   keyLen ivLen <=8
void DES_cbc_decrypt(const char *in, int length, char* out, int& outLen, const char* keydata, int keyLen, const char* ivdata, int ivLen);

std::string StrToHex(const unsigned char *md/*字符串*/, int nLen/*转义多少个字符*/);
void HexToStr(const char* strHex, char* out, int& outLen);

std::string DES_cbc_encrypt_base64(const char *in, int inLen, const char* keydata, int keyLen, const char* ivdata, int ivLen);
std::string DES_cbc_decrypt_base64(const char *in, int inLen, const char* keydata, int keyLen, const char* ivdata, int ivLen);
}

