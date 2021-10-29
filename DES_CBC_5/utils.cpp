//
//  utils.cpp
//  DESCBC
//

#include "utils.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <cstring>
#include <random>
#include <cctype>

uint64_t ntoh64(const uint64_t *input) {
	uint64_t rval;
	uint8_t *data = (uint8_t *) &rval;

	data[0] = (uint8_t)((*input & 0xff00000000000000) >> 56);
	data[1] = (uint8_t)((*input & 0x00ff000000000000) >> 48);
	data[2] = (uint8_t)((*input & 0x0000ff0000000000) >> 40);
	data[3] = (uint8_t)((*input & 0x000000ff00000000) >> 32);
	data[4] = (uint8_t)((*input & 0x00000000ff000000) >> 24);
	data[5] = (uint8_t)((*input & 0x0000000000ff0000) >> 16);
	data[6] = (uint8_t)((*input & 0x000000000000ff00) >> 8  );
	data[7] = (uint8_t)((*input & 0x00000000000000ff) >> 0  );

	return rval;
}

uint64_t hton64(const uint64_t *input) {
	return (ntoh64(input));
}

void print_hex_string(string label, uint64_t &input) {
    cout << label;
    cout.fill('0');
    cout.width(16);
    cout << hex << uppercase << input << endl;
}

uint8_t valid_hex_string(string &data, int len) {
    for (int i = 0; i < len; ++i) {
        if (!isxdigit(data[i])) {
            return 0;
        }
    }
    return 1;
}

uint64_t DES_key_iv_check(const char *data, uint64_t length) {
    string str(data);
    string padded = str.append(length * 2 - strlen(data), '0');
    return strtoull(str.c_str(), 0, 16);
}

string DES_random_string(const int bytes) {
    string s;
    s.reserve(bytes * 2);
    srand((int)time(0));  // 产生随机种子
    for (int i = 0; i < bytes * 2; ++i) {
        s += HEX_SET[rand()% sizeof(HEX_SET)];
    }
    return s;
}
