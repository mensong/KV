#include <cstdio>
#include <cstdlib>
#include "DesHelper.h"
#include "utils.h"
#include "cipher_des.h"
#include "cipher_padding.h"
#include <fstream>
#include "base64.h"

using namespace std;

namespace GL
{

// Encrypt the message contained in input file with iv and key using DES in CBC mode
void DES_cbc_encrypt(const std::string& inputFile, const std::string& outputFile, uint64_t(&subkeys)[ITERATIONS], uint64_t &iv)
{
	int enc = DES_ENCRYPT;

	ifstream in;
	ofstream out;

	in.open(inputFile.c_str(), ios::binary | ios::in | ios::ate);
	out.open(outputFile.c_str(), ios::binary | ios::out);
	if (!in) {
		cerr << "Error: missing file " << inputFile << endl;
		exit(1);
	}

	uint64_t len = in.tellg();

	in.seekg(0, ios::beg);
	uint64_t buffer = 0;

	// start CBC mode
	uint64_t c_prev = iv;
	for (uint64_t i = 0; i < len / 8; ++i) {
		in.read((char *)&buffer, 8);
		uint64_t p_curr = hton64(&buffer);
		c_prev = des(subkeys, p_curr ^ c_prev, enc);
		uint64_t x = hton64(&c_prev);
		out.write((char *)&x, 8);
	}

	// last block: perform PKCS5 Padding if necessary 
	uint64_t padlen = get_pad_length(len);
	if (padlen == 8) {
		uint64_t p_curr = 0x0808080808080808;
		c_prev = des(subkeys, p_curr ^ c_prev, enc);
		uint64_t x = hton64(&c_prev);
		out.write((char *)&x, 8);
		//print_hex_string(c_prev);
	}
	else {
		buffer = 0;
		in.read((char *)&buffer, len % 8);
		uint64_t p_curr = hton64(&buffer);
		p_curr = pad_with_length(p_curr, padlen);
		c_prev = des(subkeys, p_curr ^ c_prev, enc);
		uint64_t x = hton64(&c_prev);
		out.write((char *)&x, 8);
		//print_hex_string(c_prev);
	}
	in.close();
	out.close();
}

// Decrypt the message contained in input file with iv and key using DES in CBC mode
void DES_cbc_decrypt(const std::string& inputFile, const std::string& outputFile, uint64_t(&subkeys)[ITERATIONS], uint64_t &iv)
{
	int dec = DES_DECRYPT;

	ifstream in;
	ofstream out;

	in.open(inputFile.c_str(), ios::binary | ios::in | ios::ate);
	out.open(outputFile.c_str(), ios::binary | ios::out);
	if (!in) {
		cout << "Error: missing file " << inputFile << endl;
		exit(1);
	}

	uint64_t length = in.tellg();
	in.seekg(0, ios::beg);
	uint64_t buffer = 0;

	uint64_t c_prev = iv;
	for (uint64_t i = 0; i < length / 8 - 1; ++i) {
		in.read((char *)&buffer, 8);
		uint64_t p_curr = hton64(&buffer);
		uint64_t res = des(subkeys, p_curr, dec) ^ c_prev;
		uint64_t x = hton64(&res);
		out.write((char *)&x, 8);
		c_prev = p_curr;
	}

	// After decrypting, remove padding
	buffer = 0;
	// Read last line of file
	in.read((char *)&buffer, 8);
	uint64_t p_curr = hton64(&buffer);
	uint64_t res = des(subkeys, p_curr, dec) ^ c_prev;

	// last byte: pad value
	int padlen = (res & 0xFF);

	if (padlen < 8)
	{
		res = remove_pad(res, padlen);
		uint64_t x = hton64(&res);
		x >>= padlen * 8;
		out.write((char *)&x, 8 - padlen);
	}
	in.close();
	out.close();
}

// Encrypt the message contained in input file with iv and key using DES in CBC mode
void DES_cbc_encrypt(const char *in, int inLen, char* out, int& outLen, uint64_t(&subkeys)[ITERATIONS], uint64_t &iv)
{
	int enc = DES_ENCRYPT;
	uint64_t buffer = 0;
	outLen = 0;

	// start CBC mode
	uint64_t c_prev = iv;
	for (int i = 0; i < inLen / 8; ++i)
	{
		memcpy(((char *)&buffer), in + outLen, 8);
		uint64_t p_curr = hton64(&buffer);
		c_prev = des(subkeys, p_curr ^ c_prev, enc);
		uint64_t x = hton64(&c_prev);
		memcpy(out + outLen, (char *)&x, 8);
		outLen += 8;
	}

	// last block: perform PKCS5 Padding if necessary 
	uint64_t padlen = get_pad_length(inLen);
	if (padlen == 8)
	{
		uint64_t p_curr = 0x0808080808080808;
		c_prev = des(subkeys, p_curr ^ c_prev, enc);
		uint64_t x = hton64(&c_prev);
		//out.write((char *)&x, 8);
		memcpy(out + outLen, (char *)&x, 8);
		outLen += 8;
		//print_hex_string(c_prev);
	}
	else
	{
		buffer = 0;
		memcpy(((char *)&buffer), in + outLen, inLen % 8);
		uint64_t p_curr = hton64(&buffer);
		p_curr = pad_with_length(p_curr, padlen);
		c_prev = des(subkeys, p_curr ^ c_prev, enc);
		uint64_t x = hton64(&c_prev);
		//out.write((char *)&x, 8);
		memcpy(out + outLen, (char *)&x, 8);
		outLen += 8;
		//print_hex_string(c_prev);
	}
}

// Decrypt the message contained in input file with iv and key using DES in CBC mode
void DES_cbc_decrypt(const char *in, int length, char* out, int& outLen, uint64_t(&subkeys)[ITERATIONS], uint64_t &iv)
{
	int dec = DES_DECRYPT;
	uint64_t buffer = 0;
	outLen = 0;

	uint64_t c_prev = iv;
	for (int i = 0; i < length / 8 - 1; ++i)
	{
		memcpy(((char *)&buffer), in + outLen, 8);
		uint64_t p_curr = hton64(&buffer);
		uint64_t res = des(subkeys, p_curr, dec) ^ c_prev;
		uint64_t x = hton64(&res);
		//out.write((char *)&x, 8);
		memcpy(out + outLen, (char *)&x, 8);
		outLen += 8;
		c_prev = p_curr;
	}

	// After decrypting, remove padding
	buffer = 0;
	// Read last line of file
	memcpy(((char *)&buffer), in + outLen, 8);
	uint64_t p_curr = hton64(&buffer);
	uint64_t res = des(subkeys, p_curr, dec) ^ c_prev;

	// last byte: pad value
	int padlen = (res & 0xFF);

	if (padlen < 8)
	{
		res = remove_pad(res, padlen);
		uint64_t x = hton64(&res);
		x >>= padlen * 8;
		memcpy(out + outLen, (char *)&x, 8 - padlen);
		outLen += 8 - padlen;
	}
}

static const char hex_chars[] = "0123456789ABCDEF";
std::string StrToHex(const unsigned char *md/*字符串*/, int nLen/*转义多少个字符*/)
{
	std::string strSha1;

	unsigned int c = 0;

	// 查看unsigned char占几个字节 
	// 实际占1个字节，8位 
	int nByte = sizeof(unsigned char);

	for (int i = 0; i < nLen; i++)
	{
		// 查看md一个字节里的信息  
		unsigned int x = 0;
		x = md[i];
		x = md[i] >> 4;// 右移，干掉4位，左边高位补0000

		c = (md[i] >> 4) & 0x0f;
		strSha1 += hex_chars[c];
		strSha1 += hex_chars[md[i] & 0x0f];
	}
	return strSha1;
}

void HexToStr(const char* strHex, char* out, int& outLen)
{
	outLen = 0;
	int iLenth = (int)strlen(strHex);

	int i = 0;
	int iOcx;
	for (int i = 0; i < iLenth / 2; i++)
	{
		if (1 != sscanf(strHex + (i * 2), "%2x", &iOcx))
			break;
		out[i] = (char)iOcx;
		++outLen;
	}
}

std::string DES_cbc_encrypt_base64(const char *in, int inLen, const char* keydata, int keyLen, const char* ivdata, int ivLen)
{
	int mmlen = inLen * 2 + 7;
	char* mmCode = new char[mmlen];
	int nOutLen = 0;
	GL::DES_cbc_encrypt(in, inLen, mmCode, nOutLen, keydata, keyLen, ivdata, ivLen);
	if (nOutLen <= 0)
	{
		delete[] mmCode;
		return "";
	}

	std::string szBase64(nOutLen * 3, '\0');
	int nBase64 = 0;
	if (!base64_encode((const unsigned char*)mmCode, nOutLen, const_cast<char*>(szBase64.data()), nBase64))
	{
		delete[] mmCode;
		return "";
	}
	delete[] mmCode;

	return szBase64.c_str();
}

std::string DES_cbc_decrypt_base64(const char *in, int inLen, const char* keydata, int keyLen, const char* ivdata, int ivLen)
{
	char* szDeBase64 = new char[inLen];
	int nDeBase64Len = 0;
	base64_decode(in, inLen, (unsigned char*)szDeBase64, nDeBase64Len);
	if (nDeBase64Len <= 0)
	{
		delete[] szDeBase64;
		return "";
	}

	std::string sDeDes(inLen, '\0');
	int nDeDesLen = 0;
	GL::DES_cbc_decrypt(szDeBase64, nDeBase64Len, const_cast<char*>(sDeDes.c_str()), nDeDesLen, keydata, keyLen, ivdata, ivLen);
	delete[] szDeBase64;

	return sDeDes.c_str();
}

// Encrypt the message contained in input file with iv and key using DES in CBC mode
void DES_cbc_encrypt(const char *in, int inLen, char* out, int& outLen, const char* keydata, int keyLen, const char* ivdata, int ivLen)
{
	uint64_t iv = 0x0000000000000000; // IV to use, as hexidecimal string
	uint64_t K = 0x0000000000000000; // key to use, as hexidecimal string

	std::string keyHex = StrToHex((unsigned char*)keydata, keyLen);
	std::string ivHex = StrToHex((unsigned char*)ivdata, ivLen);
	iv = DES_key_iv_check(keyHex.c_str(), 8);
	K = DES_key_iv_check(ivHex.c_str(), 8);

	/* generate 16 subkeys, each of 48 bits */
	uint64_t subkeys[ITERATIONS] = { 0 };
	key_schedule(K, subkeys);

	DES_cbc_encrypt(in, inLen, out, outLen, subkeys, iv);
}

// Decrypt the message contained in input file with iv and key using DES in CBC mode
void DES_cbc_decrypt(const char *in, int length, char* out, int& outLen, const char* keydata, int keyLen, const char* ivdata, int ivLen)
{
	uint64_t iv = 0x0000000000000000; // IV to use, as hexidecimal string
	uint64_t K = 0x0000000000000000; // key to use, as hexidecimal string

	std::string keyHex = StrToHex((unsigned char*)keydata, keyLen);
	std::string ivHex = StrToHex((unsigned char*)ivdata, ivLen);
	iv = DES_key_iv_check(keyHex.c_str(), 8);
	K = DES_key_iv_check(ivHex.c_str(), 8);

	/* generate 16 subkeys, each of 48 bits */
	uint64_t subkeys[ITERATIONS] = { 0 };
	key_schedule(K, subkeys);

	DES_cbc_decrypt(in, length, out, outLen, subkeys, iv);
}

}

