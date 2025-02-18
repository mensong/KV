#pragma once
#include <windows.h>
#include "GlobalMutex.h"

class SHM
{
public:
    SHM();
    ~SHM();

    bool Init(const TCHAR* shmName, int maxDataCount, int perDataSize);
    bool Write(const char* pData, int dataSize, int dataID);
    int Read(char* pData, int dataID);

protected:
#define INDEX_SIZE sizeof(int)
    int m_maxDataCount;
    int m_perDataSize;

    HANDLE m_hMapFile;
    char* m_pBuf;
    GlobalMutex m_mutex;
};

