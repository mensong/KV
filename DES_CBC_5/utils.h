//
//  utils.hpp
//  DESCBC
//

#ifndef utils_hpp
#define utils_hpp

#include <string>

using namespace std;

#if defined(_MSC_VER)
#define strtoll _strtoi64
#endif

#if defined(_MSC_VER)
#define strtoull _strtoui64
#endif

typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

// 64-bit ntohl(): convert values between host and big-/little-endian byte order
// https://stackoverflow.com/questions/809902/64-bit-ntohl-in-c
uint64_t ntoh64(const uint64_t *input);
uint64_t hton64(const uint64_t *input);


// printed string will be filled with leading 0s to get 8 bytes 
void print_hex_string(string label, uint64_t &input);

// generate 'random' string of len bytes, using c++11 <random> library
string DES_random_string(const int len);

// pad data in the end with 0 to length bytes
uint64_t DES_key_iv_check(const char *data, uint64_t length);

// check if string is valid hex
uint8_t valid_hex_string(string &data, int len);

const char HEX_SET[] = "0123456789abcdef";

// keys/ivs for testing
const static uint64_t cbc_keys[8] = {
        0x0123456789abcdef,
        0x3832353134313435,
        0xf1e0d3c2b5a49786,
        0xfedcba9876543210,
        0x0E329232EA6D0D73,
        0x133457799BBCDFF1,
        0x29AB9D18B2449E31, // *
        0x5E72D79A11B34FEE
};

const static uint64_t ivs[8] = {
        0x0000000000000000,
        0x133457799BBCDFF1,
        0x5E72D79A11B34FEE
};


#endif
