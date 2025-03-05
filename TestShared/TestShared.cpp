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
    const char* pIn = "1121111111111111111111111111111111111";
    int pInLen = strlen(pIn);
    while (1)
    {
        KV::Ins().SetSharedMem("MyKV", 0, pIn, pInLen);
    }
}

#ifdef _WIN32
unsigned __stdcall
#else
void*
#endif 
_process_thread1(void* arg)
{
    char* pOut = new char[512];
    while (1)
    {
        KV::Ins().GetSharedMem("MyKV", 0, pOut, 512);
    }
    delete[] pOut;
}

bool TraverseSharedMemDataIDsCallback(int dataID, void* userData)
{
    int dataLen = KV::Ins().GetSharedMem("MyKV", dataID, NULL, 0);
    return true;
}

int main(int argc, char* argv[])
{
    bool isCreated = false;
    KV::Ins().InitSharedMem("MyKV", 10000000, 64, &isCreated);
    if (isCreated)
        std::cout << "创建了MyKV" << std::endl;
    else
        std::cout << "打开了MyKV" << std::endl;
    
    KV::Ins().SetSharedMem("MyKV", 0, "mensong", 7);
    KV::Ins().SetSharedMem("MyKV", 1, "mensong1", 8);
    KV::Ins().SetSharedMem("MyKV", 2, "mensong2", 8);
    KV::Ins().SetSharedMem("MyKV", 3, "mensong3", 8);
    int n = KV::Ins().AppendSharedMem("MyKV", "mensong3", 8, 10);
    KV::Ins().GetSharedMemDataIDs("MyKV", TraverseSharedMemDataIDsCallback, NULL);

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
