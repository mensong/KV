#ifndef GLOBAL_MUTEX_H   
#define GLOBAL_MUTEX_H
#include <windows.h>

class GlobalMutex
{
public:
	GlobalMutex();
	~GlobalMutex();

	bool Init(const TCHAR* name = NULL);
	bool IsValid();
	bool Lock(DWORD waitMS = INFINITE);
	bool Unlock();

private:
	HANDLE m_hMutex;
};


#endif