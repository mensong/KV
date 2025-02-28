#pragma once

#ifndef _AFX
#include <windows.h>
#endif
#include <string>

#ifdef KV_EXPORTS
#define KV_API extern "C" __declspec(dllexport)
#else
#define KV_API extern "C" __declspec(dllimport)
#endif

KV_API bool __cdecl SetStrA(const char* k, const char* v);
KV_API bool __cdecl SetStrW(const wchar_t* k, const wchar_t* v);
KV_API const char* __cdecl GetStrA(const char* k, const char* def = "");
KV_API const wchar_t* __cdecl GetStrW(const wchar_t* k, const wchar_t* def = L"");
KV_API bool __cdecl HasStrA(const char* k);
KV_API bool __cdecl HasStrW(const wchar_t* k);
KV_API void __cdecl DelStrA(const char* k);
KV_API void __cdecl DelStrW(const wchar_t* k);
KV_API int __cdecl StrAKeysCount();
KV_API const char* __cdecl GetStrAKey(int keyIdx);
KV_API int __cdecl StrWKeysCount();
KV_API const wchar_t* __cdecl GetStrWKey(int keyIdx);

KV_API bool __cdecl SetInt(const char* k, int v);
KV_API int __cdecl GetInt(const char* k, int def = 0);
KV_API bool __cdecl HasInt(const char* k);
KV_API void __cdecl DelInt(const char* k);
KV_API int __cdecl IntKeysCount();
KV_API const char* __cdecl GetIntKey(int keyIdx);

KV_API bool __cdecl SetDouble(const char* k, double v);
KV_API double __cdecl GetDouble(const char* k, double def = 0.0);
KV_API bool __cdecl HasDouble(const char* k);
KV_API void __cdecl DelDouble(const char* k);
KV_API int __cdecl DoubleKeysCount();
KV_API const char* __cdecl GetDoubleKey(int keyIdx);

KV_API bool __cdecl SetBuff(const char* k, const char* buff, int buffLen);
KV_API const char* __cdecl GetBuff(const char* k, int& outLen);
KV_API bool __cdecl HasBuff(const char* k);
KV_API void __cdecl DelBuff(const char* k);
KV_API int __cdecl BuffKeysCount();
KV_API const char* __cdecl GetBuffKey(int keyIdx);

KV_API const char* __cdecl Encrypt(const char* data, const char* desKey);
KV_API const char* __cdecl Decrypt(const char* data, const char* desKey);
KV_API bool __cdecl InitEncryData(const char* encryFilePath = NULL);
KV_API const char* __cdecl GetDecrypt(const char* k, const char* def = "");
KV_API const char* __cdecl EncryptData(const char* k, const char* v);
KV_API int __cdecl EncryptDataKeysCount();
KV_API const char* __cdecl GetEncryptDataKey(int keyIdx);

KV_API bool __cdecl InitSharedMem(const char* globalName, int blockCount, int blockSize);
KV_API bool __cdecl SetSharedMem(const char* globalName, int dataID, const char* v);
KV_API const char* __cdecl GetSharedMem(const char* globalName, int dataID);


#define DEF_PROC(hDll, name) \
	this->name = (FN_##name)::GetProcAddress(hDll, #name)
class KV
{
public:
	typedef bool(__cdecl *FN_SetStrA)(const char* k, const char* v);
	typedef bool(__cdecl *FN_SetStrW)(const wchar_t* k, const wchar_t* v);
	typedef const char* (__cdecl *FN_GetStrA)(const char* k, const char* def);
	typedef const wchar_t* (__cdecl*FN_GetStrW)(const wchar_t* k, const wchar_t* def);
	typedef bool(__cdecl *FN_HasStrA)(const char* k);
	typedef bool(__cdecl *FN_HasStrW)(const wchar_t* k);
	typedef void(__cdecl *FN_DelStrA)(const char* k);
	typedef void(__cdecl *FN_DelStrW)(const wchar_t* k);
	typedef int(__cdecl *FN_StrAKeysCount)();
	typedef const char* (__cdecl *FN_GetStrAKey)(int keyIdx);
	typedef int(__cdecl *FN_StrWKeysCount)();
	typedef const wchar_t* (__cdecl *FN_GetStrWKey)(int keyIdx);
	typedef bool(__cdecl *FN_SetInt)(const char* k, int v);
	typedef int(__cdecl *FN_GetInt)(const char* k, int def);
	typedef bool(__cdecl *FN_HasInt)(const char* k);
	typedef void(__cdecl *FN_DelInt)(const char* k);
	typedef int(__cdecl *FN_IntKeysCount)();
	typedef const char* (__cdecl *FN_GetIntKey)(int keyIdx);
	typedef bool(__cdecl *FN_SetDouble)(const char* k, double v);
	typedef double(__cdecl *FN_GetDouble)(const char* k, double def);
	typedef bool(__cdecl *FN_HasDouble)(const char* k);
	typedef void(__cdecl *FN_DelDouble)(const char* k);
	typedef int(__cdecl *FN_DoubleKeysCount)();
	typedef const char* (__cdecl *FN_GetDoubleKey)(int keyIdx);
	typedef bool(__cdecl *FN_SetBuff)(const char* k, const char* buff, int buffLen);
	typedef const char* (__cdecl *FN_GetBuff)(const char* k, int& outLen);
	typedef bool(__cdecl *FN_HasBuff)(const char* k);
	typedef void(__cdecl *FN_DelBuff)(const char* k);
	typedef int(__cdecl *FN_BuffKeysCount)();
	typedef const char* (__cdecl *FN_GetBuffKey)(int keyIdx);
	typedef const char* (__cdecl *FN_Encrypt)(const char* data, const char* desKey);
	typedef const char* (__cdecl *FN_Decrypt)(const char* data, const char* desKey);
	typedef bool(__cdecl *FN_InitEncryData)(const char* encryFilePath);
	typedef const char* (__cdecl *FN_GetDecrypt)(const char* k, const char* def);
	typedef const char* (__cdecl *FN_EncryptData)(const char* k, const char* v);
	typedef int(__cdecl *FN_EncryptDataKeysCount)();
	typedef const char* (__cdecl *FN_GetEncryptDataKey)(int keyIdx);
	typedef bool (__cdecl *FN_InitSharedMem)(const char* globalName, int blockCount, int blockSize);
	typedef bool(__cdecl *FN_SetSharedMem)(const char* globalName, int dataID, const char* v);
	typedef const char* (__cdecl *FN_GetSharedMem)(const char* globalName, int dataID);

	KV()
	{
		hDll = LoadLibraryFromCurrentDir("KV.dll");
		if (hDll)
		{
			HMODULE __hDll__ = (hDll);
			DEF_PROC(__hDll__, SetStrA);
			DEF_PROC(__hDll__, SetStrW);
			DEF_PROC(__hDll__, GetStrA);
			DEF_PROC(__hDll__, GetStrW);
			DEF_PROC(__hDll__, HasStrA);
			DEF_PROC(__hDll__, HasStrW);
			DEF_PROC(__hDll__, DelStrA);
			DEF_PROC(__hDll__, DelStrW);
			DEF_PROC(__hDll__, StrAKeysCount);
			DEF_PROC(__hDll__, GetStrAKey);
			DEF_PROC(__hDll__, StrWKeysCount);
			DEF_PROC(__hDll__, GetStrWKey);
			DEF_PROC(__hDll__, SetInt);
			DEF_PROC(__hDll__, GetInt);
			DEF_PROC(__hDll__, HasInt);
			DEF_PROC(__hDll__, DelInt);
			DEF_PROC(__hDll__, IntKeysCount);
			DEF_PROC(__hDll__, GetIntKey);
			DEF_PROC(__hDll__, SetDouble);
			DEF_PROC(__hDll__, GetDouble);
			DEF_PROC(__hDll__, HasDouble);
			DEF_PROC(__hDll__, DelDouble);
			DEF_PROC(__hDll__, DoubleKeysCount);
			DEF_PROC(__hDll__, GetDoubleKey);
			DEF_PROC(__hDll__, SetBuff);
			DEF_PROC(__hDll__, GetBuff);
			DEF_PROC(__hDll__, HasBuff);
			DEF_PROC(__hDll__, DelBuff);
			DEF_PROC(__hDll__, BuffKeysCount);
			DEF_PROC(__hDll__, GetBuffKey);
			DEF_PROC(__hDll__, Encrypt);
			DEF_PROC(__hDll__, Decrypt);
			DEF_PROC(__hDll__, InitEncryData);
			DEF_PROC(__hDll__, GetDecrypt);
			DEF_PROC(__hDll__, EncryptData);
			DEF_PROC(__hDll__, EncryptDataKeysCount);
			DEF_PROC(__hDll__, GetEncryptDataKey);
			DEF_PROC(__hDll__, InitSharedMem);
			DEF_PROC(__hDll__, SetSharedMem);
			DEF_PROC(__hDll__, GetSharedMem);
		}
	}
		
	FN_SetStrA		 SetStrA;
	FN_SetStrW		 SetStrW;
	FN_GetStrA		 GetStrA;
	FN_GetStrW		 GetStrW;
	FN_HasStrA		 HasStrA;
	FN_HasStrW		 HasStrW;
	FN_DelStrA		 DelStrA;
	FN_DelStrW		 DelStrW;
	FN_StrAKeysCount StrAKeysCount;
	FN_GetStrAKey	 GetStrAKey;
	FN_StrWKeysCount StrWKeysCount;
	FN_GetStrWKey	 GetStrWKey;
	FN_SetInt		 SetInt;
	FN_GetInt		 GetInt;
	FN_HasInt		 HasInt;
	FN_DelInt		 DelInt;
	FN_IntKeysCount  IntKeysCount;
	FN_GetIntKey	 GetIntKey;
	FN_SetDouble	 SetDouble;
	FN_GetDouble	 GetDouble;
	FN_HasDouble	 HasDouble;
	FN_DelDouble	 DelDouble;
	FN_DoubleKeysCount  DoubleKeysCount;
	FN_GetDoubleKey	 GetDoubleKey;
	FN_SetBuff		 SetBuff;
	FN_GetBuff		 GetBuff;
	FN_HasBuff		 HasBuff;
	FN_DelBuff		 DelBuff;
	FN_BuffKeysCount BuffKeysCount;
	FN_GetBuffKey	 GetBuffKey;
	FN_Encrypt		 Encrypt;
	FN_Decrypt		 Decrypt;
	FN_InitEncryData InitEncryData;
	FN_GetDecrypt	 GetDecrypt;
	FN_EncryptData	 EncryptData;
	FN_EncryptDataKeysCount EncryptDataKeysCount;
	FN_GetEncryptDataKey GetEncryptDataKey;
	FN_InitSharedMem  InitSharedMem;
	FN_SetSharedMem  SetSharedMem;
	FN_GetSharedMem  GetSharedMem;

	static KV& Ins()
	{
		static KV s_ins;
		return s_ins;
	}

	~KV()
	{
		if (hDll)
		{
			FreeLibrary(hDll);
			hDll = NULL;
		}
	}
	static HMODULE LoadLibraryFromCurrentDir(const char* dllName)
	{
		char selfPath[MAX_PATH];
		MEMORY_BASIC_INFORMATION mbi;
		HMODULE hModule = ((::VirtualQuery(
			LoadLibraryFromCurrentDir, &mbi, sizeof(mbi)) != 0) ? (HMODULE)mbi.AllocationBase : NULL);
		::GetModuleFileNameA(hModule, selfPath, MAX_PATH);
		std::string moduleDir(selfPath);
		size_t idx = moduleDir.find_last_of('\\');
		moduleDir = moduleDir.substr(0, idx);
		char curDir[MAX_PATH];
		::GetCurrentDirectoryA(MAX_PATH, curDir);
		::SetCurrentDirectoryA(moduleDir.c_str());
		HMODULE hDll = LoadLibraryA(dllName);
		::SetCurrentDirectoryA(curDir);
		if (!hDll)
		{
			DWORD err = ::GetLastError();
			char buf[10];
			sprintf_s(buf, "%u", err);
			::MessageBoxA(NULL, ("找不到" + std::string(dllName) + "模块:" + buf).c_str(), "找不到模块", MB_OK | MB_ICONERROR);
		}
		return hDll;
	}

	HMODULE hDll;
};

#ifdef UNICODE
#define KVSetStr KV::Ins().SetStrW
#define KVGetStr KV::Ins().GetStrW
#define KVHasStr KV::Ins().HasStrW
#define KVDelStr KV::Ins().DelStrW
#else
#define KVSetStr KV::Ins().SetStrA
#define KVGetStr KV::Ins().GetStrA
#define KVHasStr KV::Ins().HasStrA
#define KVDelStr KV::Ins().DelStrA
#endif
