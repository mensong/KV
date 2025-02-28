#include "GlobalMutex.h"

GlobalMutex::GlobalMutex()
	: m_hMutex(NULL)
{
	
}

GlobalMutex::~GlobalMutex()
{
	if (!IsValid())
		return;

	Unlock();
	::CloseHandle(m_hMutex);
	m_hMutex = NULL;
}

bool GlobalMutex::Init(const TCHAR* name)
{
	if (name)
	{
		m_hMutex = NULL;
		for (int i = 0; (i < 10 && m_hMutex == NULL); i++)// try 10 times
		{
			m_hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, name);
			if (m_hMutex == INVALID_HANDLE_VALUE || m_hMutex == NULL)
			{
				m_hMutex = ::CreateMutex(NULL, FALSE, name);
				if (m_hMutex == INVALID_HANDLE_VALUE)
					m_hMutex = NULL;
			}

			if (m_hMutex == NULL)
			{
				//按当前进程ID生成等待时间
				DWORD id = GetCurrentProcessId();
				while (id > 500)
					id /= 10;
				Sleep(id);
			}
		}
	}
	else
	{
		m_hMutex = ::CreateMutex(NULL, FALSE, NULL);
		if (m_hMutex == INVALID_HANDLE_VALUE)
			m_hMutex = NULL;
	}

	return m_hMutex != NULL;
}

bool GlobalMutex::IsValid()
{
	return (m_hMutex != NULL && m_hMutex != INVALID_HANDLE_VALUE);
}

bool GlobalMutex::Lock(DWORD waitMS/* = INFINITE*/)
{
	if (!IsValid())
		return false;

	DWORD res = WaitForSingleObject(m_hMutex, waitMS);
	switch (res)
	{
	case WAIT_OBJECT_0:
	case STATUS_ABANDONED_WAIT_0:
		return true;
		break;
	case WAIT_TIMEOUT:
		return false;
		break;
	case WAIT_FAILED:
		return false;
		break;
	}
	return false;
}

bool GlobalMutex::Unlock()
{
	if (!IsValid())
		return false;
	return ReleaseMutex(m_hMutex) == TRUE;
}
