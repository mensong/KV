// TestShared.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <process.h>
#include "../KV/KV.h"

//SHM shm;

#ifdef _WIN32
unsigned __stdcall
#else
void*
#endif 
_process_thread(void* arg)
{
    while (1)
    {
        KV::Ins().SetSharedMem("MyKV", 0, "mensong");
        Sleep(1);
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
        KV::Ins().GetSharedMem("MyKV", 0);
        Sleep(1);
    }
}

int main(int argc, char* argv[])
{
    KV::Ins().InitSharedMem("MyKV", 10000000, 512);
    
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
