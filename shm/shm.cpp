#include "shm.h"

SHM::SHM()
    : m_hMapFile(NULL)
    , m_pBuf(NULL)
    , m_maxDataCount(100)
    , m_perDataSize(512)
{
}

SHM::~SHM()
{
    //FlushViewOfFile(lpMapAddr, strTest.length() + 1);

    if (m_pBuf)
    {
        UnmapViewOfFile(m_pBuf);
        m_pBuf = NULL;
    }
    if (m_hMapFile && m_hMapFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hMapFile);
        m_hMapFile = NULL;
    }
}

bool SHM::Init(const TCHAR* shmName, int maxDataCount, int perDataSize)
{
    TCHAR intialLockName[MAX_PATH] = TEXT("");
    lstrcat(intialLockName, TEXT("Init_"));
    lstrcat(intialLockName, shmName);
    GlobalMutex initLocker;
    initLocker.Init();
    initLocker.Lock();

    m_maxDataCount = maxDataCount;
    m_perDataSize = perDataSize;

    bool created = false;
    //打开共享的文件对象。
    m_hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS, 
        FALSE, 
        shmName);
    if (!m_hMapFile || m_hMapFile == INVALID_HANDLE_VALUE)
    {
        //创建共享文件。
        m_hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE, 
            NULL, 
            PAGE_READWRITE/*物理页*/, 
            0/*高位*/,
            maxDataCount * (perDataSize + INDEX_SIZE)/*低位*/,
            shmName);
        created = true;
    }

    if (!m_hMapFile || m_hMapFile == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    //拷贝数据到共享文件里。
    m_pBuf = (char*)MapViewOfFile(
        m_hMapFile,
        FILE_MAP_ALL_ACCESS,
        0, 0, 0);
    if (!m_pBuf)
    {
        CloseHandle(m_hMapFile);
        m_hMapFile = NULL;
        return false;
    }
    if (created)
    {
        memset(m_pBuf, 0, maxDataCount * (perDataSize + INDEX_SIZE));
    }

    TCHAR dataLockName[MAX_PATH] = TEXT("");
    lstrcat(dataLockName, TEXT("Data_"));
    lstrcat(dataLockName, shmName);
    m_mutex.Init(dataLockName);

	return true;
}

bool SHM::Write(const char* pData, int dataSize, int dataID)
{
    if (dataID >= m_maxDataCount || dataSize > m_perDataSize)
		return false;

    m_mutex.Lock();

    char* p = m_pBuf + (dataID * (m_perDataSize + INDEX_SIZE));
    memcpy(p, (PVOID)&dataSize, INDEX_SIZE);
    memcpy(p + INDEX_SIZE, pData, dataSize);

    m_mutex.Unlock();
	return true;
}

int SHM::Read(char* pData, int dataID)
{
    if (dataID >= m_maxDataCount)
        return -1;

    m_mutex.Lock();

    char* p = m_pBuf + (dataID * (m_perDataSize + INDEX_SIZE));
    int dataSize = 0;
    memcpy((PVOID)&dataSize, p, INDEX_SIZE);
    if (pData)
		memcpy(pData, p + INDEX_SIZE, dataSize);

    m_mutex.Unlock();
	return dataSize;
}
