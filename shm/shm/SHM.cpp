#include "shm.h"
#include <set>


SHM::SHM()
    : m_hMapFile(NULL)
    , m_pBuf(NULL)
	, m_pIndexBuf(NULL)
	, m_pBlockBuf(NULL)
    , m_indexBufSize(0)
    , m_blockCount(100)
    , m_blockSize(512)
    , m_lastUsedIdx(-1)
{
}

SHM::~SHM()
{
    //FlushViewOfFile(lpMapAddr, strTest.length() + 1);

    if (m_pBuf)
    {
        UnmapViewOfFile(m_pBuf);
        m_pBuf = NULL;
        m_pIndexBuf = NULL;
        m_pBlockBuf = NULL;
    }
    if (m_hMapFile && m_hMapFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hMapFile);
        m_hMapFile = NULL;
    }
}

bool SHM::Init(const TCHAR* shmName, int blockCount, int blockSize)
{
    TCHAR intialLockName[MAX_PATH] = TEXT("");
    lstrcat(intialLockName, TEXT("Init_"));
    lstrcat(intialLockName, shmName);
    GlobalMutex initLocker;
    initLocker.Init();
    initLocker.Lock();

    m_blockCount = blockCount;
    m_blockSize = blockSize;

    //索引所需的空间
    m_indexBufSize = 
        m_blockCount * 3 + //-1 dataID 0 1 2 -1 3 -1 4 5 -1 -1
        2;                //最后两个-1 -1


    LARGE_INTEGER allocSize;
    allocSize.QuadPart =
        m_indexBufSize * sizeof(int) + //block索引号空间大小
        blockCount * sizeof(int) + (blockCount * (blockSize * sizeof(char))); //存储数据大小的空间 + block的数据空间大小

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
            allocSize.HighPart/*高位*/,
            allocSize.LowPart/*低位*/,
            shmName);
        //DWORD err = GetLastError();
        created = true;
    }

    if (!m_hMapFile || m_hMapFile == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    //拷贝索引与block数据到共享文件里。
    m_pBuf = (char*)MapViewOfFile(
        m_hMapFile,
        FILE_MAP_ALL_ACCESS,
        0, 
        0, 
        allocSize.QuadPart);
    if (!m_pBuf)
    {
        CloseHandle(m_hMapFile);
        m_hMapFile = NULL;
        return false;
    }
    if (created)
    {
        memset(m_pBuf, 0, allocSize.QuadPart);
    }

	m_pIndexBuf = (int*)m_pBuf;
    for (int i = 0; i < m_indexBufSize; i++)
        m_pIndexBuf[i] = -1;

	m_pBlockBuf = m_pBuf + (m_indexBufSize * sizeof(int));

    TCHAR dataLockName[MAX_PATH] = TEXT("");
    lstrcat(dataLockName, TEXT("Data_"));
    lstrcat(dataLockName, shmName);
    m_mutex.Init(dataLockName);

	return true;
}

bool SHM::Write(const char* pData, int dataSize, int dataID)
{
    if (dataID >= m_blockCount)
		return false;

    m_mutex.Lock();

    std::map<int, std::vector<int>> allIndexs;
	getAllIndeies(allIndexs);
    allIndexs.erase(dataID);

	const char* pDataToWrite = pData;
	int writeSize = dataSize;

    //写大于一个block的数据
    while (writeSize > m_blockSize)
    {
        int newIdx = getNoUsedIdx(allIndexs, m_lastUsedIdx + 1);
        if (newIdx < 0)
            return false;

        char* p = m_pBlockBuf + (newIdx * (sizeof(int) + m_blockSize));
		memcpy(p, (const void*)&m_blockSize, sizeof(int));
		memcpy(p + sizeof(int), pDataToWrite, m_blockSize);
        allIndexs[dataID].push_back(newIdx);

        writeSize -= m_blockSize;
        pDataToWrite += m_blockSize;
        m_lastUsedIdx = newIdx;
    }
    //写小于一个block的数据
    if (writeSize > 0)
    {
		int newIdx = getNoUsedIdx(allIndexs, m_lastUsedIdx + 1);
		if (newIdx < 0)
			return false;

		char* p = m_pBlockBuf + (newIdx * (sizeof(int) + m_blockSize));
		memcpy(p, (const void*)&writeSize, sizeof(int));
		memcpy(p + sizeof(int), pDataToWrite, writeSize);
        allIndexs[dataID].push_back(newIdx);

        m_lastUsedIdx = newIdx;
    }

    //重写indexes data
    int w = 0;
	for (auto it = allIndexs.begin(); it != allIndexs.end(); ++it)
    {
        //-1 dataID
		m_pIndexBuf[w++] = -1; 
        m_pIndexBuf[w++] = it->first;

        auto& indexs = it->second;
        for (size_t i = 0; i < indexs.size(); i++)
        {
            m_pIndexBuf[w++] = indexs[i];
        }
    }
    //结束符 -1 -1
    m_pIndexBuf[w++] = -1; 
    m_pIndexBuf[w++] = -1;

    m_mutex.Unlock();
	return true;
}

int SHM::Read(char* pOutBuf, int dataID)
{
    if (dataID >= m_blockCount)
        return -1;

    m_mutex.Lock();

	std::vector<int> indexs;
	getIndeies(dataID, indexs);

    int dataSizeTotal = 0;
    for (size_t i = 0; i < indexs.size(); i++)
    {
		char* p = m_pBlockBuf + ((indexs[i]) * (sizeof(int) + m_blockSize));

		int dataSize = 0;
		memcpy((PVOID)&dataSize, p, sizeof(int));

        if (pOutBuf)
        {
            memcpy(pOutBuf + dataSizeTotal, p + sizeof(int), dataSize);
        }

        dataSizeTotal += dataSize;
    }
    
    m_mutex.Unlock();
	return dataSizeTotal;
}

bool SHM::Remove(int dataID)
{
	if (dataID >= m_blockCount)
		return false;

	m_mutex.Lock();
    
	std::map<int, std::vector<int>> allIndexs;
	getAllIndeies(allIndexs);

    //直接删除索引
	allIndexs.erase(dataID);

	//重写indexes data
	int w = 0;
	for (auto it = allIndexs.begin(); it != allIndexs.end(); ++it)
	{
		//-1 dataID
		m_pIndexBuf[w++] = -1;
		m_pIndexBuf[w++] = it->first;

		auto& indexs = it->second;
		for (size_t i = 0; i < indexs.size(); i++)
		{
			m_pIndexBuf[w++] = indexs[i];
		}
	}
	//结束符 -1 -1
	m_pIndexBuf[w++] = -1;
	m_pIndexBuf[w++] = -1;

	m_mutex.Unlock();
    return true;
}

void SHM::getIndeies(int dataID, std::vector<int>& idxs)
{
	bool isLastFlag = false;//上次是否是-1
    int idx = 0;
    bool isInValidDataID = false;
	for (int i = 0; i < m_indexBufSize; i++)
	{
		int n = m_pIndexBuf[i];

		if (n == -1)
		{//遇到flag
			if (isLastFlag)
			{//连续遇到两个-1代表结束
				return;
			}
			else //if (!isLastFlag)
			{//遇到开始标志
				isLastFlag = true;

                if (isInValidDataID)
                {//已经读取过了，不需要再向下读取
                    return;
                }
			}
		}
		else //if (n != -1)
		{//遇到非flag
			if (isLastFlag)
			{//上次是一个-1，则代表是dataID
                if (n == dataID)
                {
                    isInValidDataID = true;
                }
			}
			else if (isInValidDataID)
			{//是有效的索引数据
                idxs.push_back(n);
			}

			isLastFlag = false;
		}
	}
}

void SHM::getAllIndeies(std::map<int, std::vector<int>>& idxs)
{
    int dataID = -1;//当前的dataID
    bool isLastFlag = false;//上次是否是-1
    for (int i = 0; i < m_indexBufSize; i++)
    {
        int n = m_pIndexBuf[i];

        if (n == -1)
        {//遇到flag
            if (isLastFlag)
            {//连续遇到两个-1代表结束
                return;
            }
            else //if (!isLastFlag)
			{//遇到开始标志
				isLastFlag = true;
				dataID = -1;
            }
        }
        else //if (n != -1)
        {//遇到非flag
            if (isLastFlag)
            {//上次是一个-1，则代表是dataID
                dataID = n;
            }
            else if (dataID != -1)
            {//是索引数据
                idxs[dataID].push_back(n);
            }

            isLastFlag = false;
        }
    }
}

int SHM::getNoUsedIdx(const std::map<int, std::vector<int>>& usedIdxs, int startIdx)
{
    std::set<int> _usedIdxs;
	for (auto it = usedIdxs.begin(); it != usedIdxs.end(); ++it)
    {
        for (size_t i = 0; i < it->second.size(); i++)
        {
            _usedIdxs.insert(it->second[i]);
        }
    }

    int idxSpaceSize = m_blockCount;
    for (size_t i = startIdx; i < idxSpaceSize; i++)
    {
        if (_usedIdxs.find(i) == _usedIdxs.end())
        {
            return i;
        }
    }

	for (size_t i = 0; i < startIdx; i++)
	{
		if (_usedIdxs.find(i) == _usedIdxs.end())
		{
			return i;
		}
	}

    return -1;
}
