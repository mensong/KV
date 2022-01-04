#pragma once

#ifdef KV_EXPORTS
#define KV_API extern "C" __declspec(dllexport)
#else
#define KV_API extern "C" __declspec(dllimport)
#endif

KV_API bool SetStrA(const char* k, const char* v);
KV_API bool SetStrW(const wchar_t* k, const wchar_t* v);
KV_API const char* GetStrA(const char* k, const char* def = "");
KV_API const wchar_t* GetStrW(const wchar_t* k, const wchar_t* def = L"");
KV_API bool HasStrA(const char* k);
KV_API bool HasStrW(const wchar_t* k);
KV_API void DelStrA(const char* k);
KV_API void DelStrW(const wchar_t* k);
KV_API int StrAKeysCount();
KV_API const char* GetStrAKey(int keyIdx);
KV_API int StrWKeysCount();
KV_API const wchar_t* GetStrWKey(int keyIdx);

KV_API bool SetInt(const char* k, int v);
KV_API int GetInt(const char* k, int def = 0);
KV_API bool HasInt(const char* k);
KV_API void DelInt(const char* k);
KV_API int IntKeysCount();
KV_API const char* GetIntKey(int keyIdx);

KV_API bool SetDouble(const char* k, double v);
KV_API double GetDouble(const char* k, double def = 0.0);
KV_API bool HasDouble(const char* k);
KV_API void DelDouble(const char* k);
KV_API int DoubleKeysCount();
KV_API const char* GetDoubleKey(int keyIdx);

KV_API bool SetBuff(const char* k, const char* buff, int buffLen);
KV_API const char* GetBuff(const char* k, int& outLen);
KV_API bool HasBuff(const char* k);
KV_API void DelBuff(const char* k);
KV_API int BuffKeysCount();
KV_API const char* GetBuffKey(int keyIdx);

KV_API const char* Encrypt(const char* data, const char* desKey);
KV_API const char* Decrypt(const char* data, const char* desKey);
KV_API bool InitEncryData(const char* encryFilePath = NULL);
KV_API const char* GetDecrypt(const char* k, const char* def = "");
KV_API const char* EncryptData(const char* k, const char* v);
KV_API int EncryptDataKeysCount();
KV_API const char* GetEncryptDataKey(int keyIdx);

KV_API bool SetSharedMem(const char* k, const char* v);
KV_API const char* GetSharedMem(const char* k, const char* def = "");
KV_API void DelSharedMem(const char* k);
KV_API int SharedMemKeysCount();
KV_API const char* GetSharedMemKey(int keyIdx);

#ifndef KV_EXPORTS
typedef bool(*FN_SetStrA)(const char* k, const char* v); 
typedef bool(*FN_SetStrW)(const wchar_t* k, const wchar_t* v); 
typedef const char* (*FN_GetStrA)(const char* k, const char* def);
typedef const wchar_t* (*FN_GetStrW)(const wchar_t* k, const wchar_t* def);
typedef bool(*FN_HasStrA)(const char* k); 
typedef bool(*FN_HasStrW)(const wchar_t* k); 
typedef void(*FN_DelStrA)(const char* k); 
typedef void(*FN_DelStrW)(const wchar_t* k);
typedef int(*FN_StrAKeysCount)();
typedef const char* (*FN_GetStrAKey)(int keyIdx);
typedef int(*FN_StrWKeysCount)();
typedef const wchar_t* (*FN_GetStrWKey)(int keyIdx);
typedef bool(*FN_SetInt)(const char* k, int v); 
typedef int(*FN_GetInt)(const char* k, int def);
typedef bool(*FN_HasInt)(const char* k); 
typedef void(*FN_DelInt)(const char* k);
typedef int(*FN_IntKeysCount)();
typedef const char* (*FN_GetIntKey)(int keyIdx);
typedef bool(*FN_SetDouble)(const char* k, double v); 
typedef double(*FN_GetDouble)(const char* k, double def);
typedef bool(*FN_HasDouble)(const char* k); 
typedef void(*FN_DelDouble)(const char* k);
typedef int(*FN_DoubleKeysCount)();
typedef const char* (*FN_GetDoubleKey)(int keyIdx);
typedef bool(*FN_SetBuff)(const char* k, const char* buff, int buffLen); 
typedef const char* (*FN_GetBuff)(const char* k, int& outLen); 
typedef bool(*FN_HasBuff)(const char* k); 
typedef void(*FN_DelBuff)(const char* k);
typedef int(*FN_BuffKeysCount)();
typedef const char* (*FN_GetBuffKey)(int keyIdx);
typedef const char* (*FN_Encrypt)(const char* data, const char* desKey);
typedef const char* (*FN_Decrypt)(const char* data, const char* desKey);
typedef bool(*FN_InitEncryData)(const char* encryFilePath);
typedef const char* (*FN_GetDecrypt)(const char* k, const char* def);
typedef const char* (*FN_EncryptData)(const char* k, const char* v);
typedef int(*FN_EncryptDataKeysCount)();
typedef const char* (*FN_GetEncryptDataKey)(int keyIdx);
typedef bool(*FN_SetSharedMem)(const char* k, const char* v);
typedef const char* (*FN_GetSharedMem)(const char* k, const char* def);
typedef void(*FN_DelSharedMem)(const char* k);
typedef int(*FN_SharedMemKeysCount)();
typedef const char* (*FN_GetSharedMemKey)(int keyIdx);

#define DEF_PROC(hDll, name) \
	FN_##name name = (FN_##name)::GetProcAddress(hDll, #name)

#define INIT_KV(hDll) \
	HMODULE __hDll__ = (hDll); \
	DEF_PROC(__hDll__, SetStrA); \
	DEF_PROC(__hDll__, SetStrW); \
	DEF_PROC(__hDll__, GetStrA); \
	DEF_PROC(__hDll__, GetStrW); \
	DEF_PROC(__hDll__, HasStrA); \
	DEF_PROC(__hDll__, HasStrW); \
	DEF_PROC(__hDll__, DelStrA); \
	DEF_PROC(__hDll__, DelStrW); \
	DEF_PROC(__hDll__, StrAKeysCount); \
	DEF_PROC(__hDll__, GetStrAKey); \
	DEF_PROC(__hDll__, StrWKeysCount); \
	DEF_PROC(__hDll__, GetStrWKey); \
	DEF_PROC(__hDll__, SetInt); \
	DEF_PROC(__hDll__, GetInt); \
	DEF_PROC(__hDll__, HasInt); \
	DEF_PROC(__hDll__, DelInt); \
	DEF_PROC(__hDll__, IntKeysCount); \
	DEF_PROC(__hDll__, GetIntKey); \
	DEF_PROC(__hDll__, SetDouble); \
	DEF_PROC(__hDll__, GetDouble); \
	DEF_PROC(__hDll__, HasDouble); \
	DEF_PROC(__hDll__, DelDouble); \
	DEF_PROC(__hDll__, DoubleKeysCount); \
	DEF_PROC(__hDll__, GetDoubleKey); \
	DEF_PROC(__hDll__, SetBuff); \
	DEF_PROC(__hDll__, GetBuff); \
	DEF_PROC(__hDll__, HasBuff); \
	DEF_PROC(__hDll__, DelBuff); \
	DEF_PROC(__hDll__, BuffKeysCount); \
	DEF_PROC(__hDll__, GetBuffKey); \
	DEF_PROC(__hDll__, Encrypt); \
	DEF_PROC(__hDll__, Decrypt); \
	DEF_PROC(__hDll__, InitEncryData); \
	DEF_PROC(__hDll__, GetDecrypt); \
	DEF_PROC(__hDll__, EncryptData); \
	DEF_PROC(__hDll__, EncryptDataKeysCount); \
	DEF_PROC(__hDll__, GetEncryptDataKey); \
	DEF_PROC(__hDll__, SetSharedMem); \
	DEF_PROC(__hDll__, GetSharedMem); \
	DEF_PROC(__hDll__, DelSharedMem); \
	DEF_PROC(__hDll__, SharedMemKeysCount); \
	DEF_PROC(__hDll__, GetSharedMemKey); 


class KV
{
private:
	static KV* s_ins;

public:
	static KV& Ins()
	{
		if (!s_ins)
			s_ins = new KV;
		return *s_ins;
	}

	static void Release()
	{
		if (s_ins)
		{
			delete s_ins;
			s_ins = NULL;
		}
	}
		
	KV()
	{
		hDll = LoadLibraryA("KV.dll");
		if (hDll)
		{
			INIT_KV(hDll);
			this->SetStrA = SetStrA;
			this->SetStrW = SetStrW;
			this->GetStrA = GetStrA;
			this->GetStrW = GetStrW;
			this->HasStrA = HasStrA;
			this->HasStrW = HasStrW;
			this->DelStrA = DelStrA;
			this->DelStrW = DelStrW;
			this->StrAKeysCount = StrAKeysCount;
			this->GetStrAKey = GetStrAKey;
			this->StrWKeysCount = StrWKeysCount;
			this->GetStrWKey = GetStrWKey;
			this->SetInt = SetInt;
			this->GetInt = GetInt;
			this->HasInt = HasInt;
			this->DelInt = DelInt;
			this->IntKeysCount = IntKeysCount;
			this->GetIntKey = GetIntKey;
			this->SetDouble = SetDouble;
			this->GetDouble = GetDouble;
			this->HasDouble = HasDouble;
			this->DelDouble = DelDouble;
			this->DoubleKeysCount = DoubleKeysCount;
			this->GetDoubleKey = GetDoubleKey;
			this->SetBuff = SetBuff;
			this->GetBuff = GetBuff;
			this->HasBuff = HasBuff;
			this->DelBuff = DelBuff;
			this->BuffKeysCount = BuffKeysCount;
			this->GetBuffKey = GetBuffKey;
			this->Encrypt = Encrypt;
			this->Decrypt = Decrypt;
			this->InitEncryData = InitEncryData;
			this->GetDecrypt = GetDecrypt;
			this->EncryptData = EncryptData;
			this->EncryptDataKeysCount = EncryptDataKeysCount;
			this->GetEncryptDataKey = GetEncryptDataKey;
			this->SetSharedMem = SetSharedMem;
			this->GetSharedMem = GetSharedMem;
			this->DelSharedMem = DelSharedMem;
			this->SharedMemKeysCount = SharedMemKeysCount;
			this->GetSharedMemKey = GetSharedMemKey;
		}
		else
		{
			::MessageBoxA(NULL, "找不到KV.dll模块", "找不到模块", MB_OK | MB_ICONERROR);
		}
	}
	~KV()
	{
		if (hDll)
		{
			FreeLibrary(hDll);
			hDll = NULL;
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
	FN_SetSharedMem  SetSharedMem;
	FN_GetSharedMem  GetSharedMem;
	FN_DelSharedMem  DelSharedMem;
	FN_SharedMemKeysCount SharedMemKeysCount;
	FN_GetSharedMemKey GetSharedMemKey;

	HMODULE hDll;
};
__declspec(selectany) KV* KV::s_ins = NULL;

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

#endif