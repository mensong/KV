// TestShared.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "../shm/shm.h"
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
        //const char* data = "mensong";
        //bool b = shm.Write(data, strlen(data) + 1, 0);
        KV::Ins().SetSharedMem(0, "mensong");
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
        //char mem[10] = { 0 };
        //int readed = shm.Read(mem, 0);
        KV::Ins().GetSharedMem(0);
    }
}

int main(int argc, char* argv[])
{
#if 0
    GlobalMutex gm;
    gm.Init(L"mensongMutex");
    if (gm.IsValid())
    {
        while (true)
        {
            if (gm.Lock())
            {
                std::cout << "Working" << std::endl;
                //Sleep(1000);
                gm.Unlock();
                std::cout << "Unwork" << std::endl;
                //Sleep(1000);
            }
            else
            {
                std::cout << "Lock fail" << std::endl;
            }
        }
    }
    else
    {
        std::cout << "GlobalMutex fail" << std::endl;
    }
#else

    KV::Ins().InitSharedMem("MyKV", 10, 512);
    
    for (int i = 0; i < 30; ++i)
    {
        unsigned  uiThreadID = 0;
        unsigned  uiThreadID1 = 0;
        HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, _process_thread, (void*)NULL, 0, &uiThreadID);
        HANDLE hThread1 = (HANDLE)_beginthreadex(NULL, 0, _process_thread1, (void*)NULL, 0, &uiThreadID1);
    }

    while (1)
        Sleep(10000);

#endif
	
    return 0;
}
