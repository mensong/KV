// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <map>
#include <string>
#include "..\SHM\shm\GlobalMutex.h"

extern std::map<std::string, GlobalMutex*> g_globalMutexs;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
	{
		for (auto it = g_globalMutexs.begin(); it != g_globalMutexs.end(); ++it)
		{
			delete it->second;
		}
		g_globalMutexs.clear();
		break;
	}
	}
	return TRUE;
}

