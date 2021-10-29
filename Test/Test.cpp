// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include "..\KV\KV.h"
#include <stdexcept>
#include <process.h>

#ifdef _WIN32
unsigned __stdcall
#else
void*
#endif 
_process_thread(void* arg)
{
	while (1)
	{
		KV::Ins().SetStrA("a", "a");
	}
}

#ifdef _WIN32
unsigned __stdcall
#else
void*
#endif 
_process_thread1(void* arg)
{
	while (1)
	{
		KV::Ins().GetStrA("a", "a");
	}
}

int main()
{
	std::string s = KV::Ins().EncryptData("url", "https://www.baidu.com");
	std::string s1 = KV::Ins().EncryptData("url1", "https://www.google.com");

	std::string s2 = KV::Ins().GetDecrypt("url", "");
	std::string s3 = KV::Ins().GetDecrypt("url1", "");
	std::string s4 = KV::Ins().GetDecrypt("url2", "");

	KV::Ins().SetStrA("a", "123");
	KV::Ins().SetStrW(L"b", L"234");
	bool b = KV::Ins().HasStrA("a");
	const char* v = KV::Ins().GetStrA("c", "");

	for (int i = 0; i < 30; ++i)
	{
		unsigned  uiThreadID = 0;
		unsigned  uiThreadID1 = 0;
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, _process_thread, (void*)NULL, 0, &uiThreadID);
		HANDLE hThread1 = (HANDLE)_beginthreadex(NULL, 0, _process_thread1, (void*)NULL, 0, &uiThreadID1);
	}
	
	while (1)
		Sleep(10000);

	return 0;
}

