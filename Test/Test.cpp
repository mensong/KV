// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include "..\KV\KV.h"
#include <stdexcept>

int main()
{
	KV::Ins().InitEncryData(NULL);

	std::string s = KV::Ins().EncryptData("url", "https://www.baidu.com");
	std::string s1 = KV::Ins().EncryptData("url1", "https://www.google.com");

	std::string s2 = KV::Ins().GetDecrypt("EntranceBaseUrl", "");
	std::string s3 = KV::Ins().GetDecrypt("url1", "");
	std::string s4 = KV::Ins().GetDecrypt("url2", "");

	int c1 = KV::Ins().EncryptDataKeysCount();
	std::string kk1 = KV::Ins().GetEncryptDataKey(0);

	KV::Ins().SetStrA("a", "123");
	KV::Ins().SetStrW(L"b", L"234");
	bool b = KV::Ins().HasStrA("a");
	const char* v = KV::Ins().GetStrA("c", "");

	int c = KV::Ins().StrAKeysCount();
	for (int i = 0; i < c+1; ++i) 
	{
		std::string k = KV::Ins().GetStrAKey(i);
	}


	std::string sen = KV::Ins().Encrypt("https://www.baidu.com", "12345678");
	sen = KV::Ins().Decrypt(sen.c_str(), "12345678");
	KV::Ins().GetDecrypt("url", "");

	return 0;
}

