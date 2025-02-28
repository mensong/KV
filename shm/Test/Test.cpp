// LogHost.cpp
//

#include <iostream>
#include <vector>
#include <string>
#include <process.h>
#include "../shm/SHM.h"

SHM shm;
const char* pIn = "123456789987654321123456789987654321123456789987654321123456789987654321123456789987654321123456789987654321";
size_t pInLen = 0;

#ifdef _WIN32
unsigned __stdcall
#else
void*
#endif 
_process_thread_write(void* arg)
{
	bool b = false;
	DWORD st = ::GetTickCount();
	while (true)
	{
		//if (::GetTickCount() - st > 20000)
		//	break;

		b = shm.Write(pIn, pInLen, 0);
		if (!b)
		{
			std::cout << "写数据出错" << std::endl;
		}
	}

	return 0;
}

#ifdef _WIN32
unsigned __stdcall
#else
void*
#endif 
_process_thread_read(void* arg)
{
	char* pOut = new char[pInLen + 1];
	DWORD st = ::GetTickCount();
	while (true)
	{
		//if (::GetTickCount() - st > 20000)
		//	break;

		int n = shm.Read(NULL, 0);
		if (n == 0)
		{
			std::cout << "读取出错" << std::endl;
			continue;
		}		
		n = shm.Read(pOut, 0);
		pOut[n] = 0;
	}
	delete[] pOut;

	return 0;
}


int main(int argc, char** argv)
{
	if (!shm.Init(L"mensong", 20000, 64))
	{
		std::cout << "Init error." << std::endl;
		return -1;
	}

	pInLen = strlen(pIn);

	std::cout << "正在测试单线程读写中..." << std::endl;

	int testCount = 10000;

	bool b = false;
	DWORD st = ::GetTickCount();
	for (int i = 0; i < testCount; ++i)
	{
		b = shm.Write(pIn, pInLen, i);
		if (!b)
		{
			std::cout << "写数据出错" << std::endl;
		}
	}
	DWORD t = ::GetTickCount() - st;
	std::cout << "Write " << testCount << "次耗时:" << t << "毫秒" << std::endl;
	std::cout << "Write 速度:" << (double)testCount / (t / 1000.0) << " 次/秒" << std::endl;
	//b = shm.Remove(0);

	st = ::GetTickCount();	
	for (size_t i = 0; i < testCount; i++)
	{
		int n = shm.Read(NULL, i);
		if (n == 0)
		{
			std::cout << "读取出错" << std::endl;
			continue;
		}
		char* pOut = new char[n + 1];
		n = shm.Read(pOut, i);
		pOut[n] = 0;
		//std::cout << pOut << std::endl;
		if (strcmp(pIn, pOut) != 0)
		{
			std::cout << "测试不通过" << std::endl;
		}
		
		delete[] pOut;
	}
	t = ::GetTickCount() - st;
	std::cout << "Read " << testCount << "次耗时:" << t << "毫秒" << std::endl;
	std::string sSpeed = (t == 0 ? 
		("大于" + std::to_string(testCount)) : 
		std::to_string((double)testCount / (t / 1000.0)));
	std::cout << "Read 速度:" << sSpeed << " 次/秒" << std::endl;

	//多线程测试
	for (int i = 0; i < 16; ++i)
	{
		unsigned  uiThreadID = 0;
		unsigned  uiThreadID1 = 0;
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, _process_thread_write, (void*)NULL, 0, &uiThreadID);
		HANDLE hThread1 = (HANDLE)_beginthreadex(NULL, 0, _process_thread_read, (void*)NULL, 0, &uiThreadID1);
	}

	std::cout << "正在测试多线程读写中，按回车键退出.";
    getchar();

	return 0;
}
