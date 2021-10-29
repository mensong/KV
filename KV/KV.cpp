// KV.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "KV.h"
#include <map>
#include <string>
#include <vector>
#include <mutex>
#include <fstream>
#include <..\crt\src\vcruntime\internal_shared.h>
#include "..\DES_CBC_5\base64.h"
#include "..\DES_CBC_5\DesHelper.h"

#define REVERT_KEY "KV.DLL"
#define DES_META "..KV.DLL"//要求8位 

std::mutex g_mt_strA;
std::map<std::string, std::string> g_strA;

std::mutex g_mt_strW;
std::map<std::wstring, std::wstring> g_strW;

std::mutex g_mt_int;
std::map<std::string, int> g_int;

std::mutex g_mt_double;
std::map<std::string, double> g_double;

std::mutex g_mt_buff;
std::map<std::string, std::vector<char> > g_buff;

std::mutex g_mt_dllPath;
std::string g_dllPath;

std::mutex g_mt_decryptData;
std::map<std::string, std::string> g_decryptData;

std::mutex g_mt_encryptData;
std::string g_encryptData;

const char* GetDllPath()
{	
	if (g_dllPath.size() != 0)
		return g_dllPath.c_str();

	//std::lock_guard<std::mutex> _lock(g_mt_dllPath);
	HMODULE hModule = reinterpret_cast<HMODULE>(&__ImageBase);
	char szPathBuffer[MAX_PATH + 1] = { 0 };
	::GetModuleFileNameA(hModule, szPathBuffer, MAX_PATH);
	int len = strlen(szPathBuffer);
	for (int i = len - 1; i > 0; --i) 
	{
		if (szPathBuffer[i] == '\\' || szPathBuffer[i] == '/')
		{
			szPathBuffer[i] = '\0';
			g_dllPath = szPathBuffer;
			break;
		}
	}
	return g_dllPath.c_str();//返回DLL自身的绝对路径
}

//in out 的长度必须为len
void revertStr(const char* key, int keyLen, const char* in, char* out, int len)
{
	for (int i = 0; i < len; ++i)
	{
		out[i] = key[i%keyLen] ^ in[i];
	}
}

std::string decrypt(const char* str64, int len64, const char* revertKey, const char* desKey)
{
	int lenDesKey = strlen(desKey);
	char* enkey = new char[lenDesKey];
	revertStr(revertKey, strlen(revertKey), desKey, enkey, lenDesKey);

	std::string s = GL::DES_cbc_decrypt_base64(str64, len64, enkey, lenDesKey, enkey, lenDesKey);
	return s;
}

std::string encrypt(const char* in, int inlen, const char* revertKey, const char* desKey)
{
	int lenDesKey = strlen(desKey);
	char* enkey = new char[lenDesKey];
	revertStr(revertKey, strlen(revertKey), desKey, enkey, lenDesKey);

	std::string s = GL::DES_cbc_encrypt_base64(in, inlen, enkey, lenDesKey, enkey, lenDesKey);

#ifdef _DEBUG
	std::string sDebug = decrypt(s.c_str(), s.length(), revertKey, desKey);
#endif

	return s;
}

void InitEncryData(const char* encryFilePath)
{
	if (g_decryptData.size() > 0)
	{
		return;
	}

	std::lock_guard<std::mutex> _lock(g_mt_decryptData);

	std::string sEncryFilePath;
	if (encryFilePath != NULL)
		sEncryFilePath = encryFilePath;
	else
		sEncryFilePath = GetDllPath() + std::string("\\KV.data");

	std::fstream fin(sEncryFilePath); //打开文件
	if (!fin.is_open())
	{
		g_decryptData.insert(std::make_pair("", ""));
		return;
	}

	std::string line;
	bool beCrack = false;//防破解
	while (std::getline(fin, line) || beCrack) //逐行读取，直到结束
	{
		DWORD ts = GetTickCount();//防破解
		if (!beCrack) //防破解
		{
			size_t idx = line.find(':');

			GetTickCount();//防破解

			if (idx == std::string::npos || idx == 0 || idx == line.size() - 1)
				continue;

			GetTickCount();//防破解

			std::string key64 = line.substr(0, idx);

			GetTickCount();//防破解

			std::string val64 = line.substr(idx + 1);

			GetTickCount();//防破解
			if (GetTickCount() - ts > 1000)//防破解
			{
				beCrack = true;//防破解
				continue;//防破解
			}

			std::string key = decrypt(key64.c_str(), key64.length(), REVERT_KEY, DES_META);
			std::string val = decrypt(val64.c_str(), val64.length(), REVERT_KEY, DES_META);
			
			g_decryptData.insert(std::make_pair(key, val));
		}
		else
		{
			g_decryptData.insert(std::make_pair(std::to_string(ts), std::to_string(ts)));//防破解
		}
	}
	fin.close();
}

KV_API const char* EncryptData(const char* k, const char* v)
{
	std::string ek = encrypt(k, strlen(k), REVERT_KEY, DES_META);
	std::string ev = encrypt(v, strlen(v), REVERT_KEY, DES_META);

	std::lock_guard<std::mutex> _lock(g_mt_encryptData);
	g_encryptData = ek + ":" + ev;
	return g_encryptData.c_str();
}

KV_API const char* GetDecrypt(const char* k, const char* def /*= ""*/)
{
	if (!k)
		return def;

	InitEncryData(NULL);

	DWORD ts = GetTickCount();//防破解

	std::lock_guard<std::mutex> _lock(g_mt_decryptData);	
	auto itFinder = g_decryptData.find(k);

	if (GetTickCount() - ts > 1000)//防破解
	{
		g_decryptData.insert(std::make_pair(std::to_string(ts), std::to_string(ts)));//防破解
		return def;//防破解
	}

	if (itFinder == g_decryptData.end())
		return def;

	return itFinder->second.c_str();
}

KV_API bool SetStrA(const char* k, const char* v)
{
	if (!k || !v)
		return false;

	std::lock_guard<std::mutex> _lock(g_mt_strA);
	g_strA[k] = v;
	return true;
}

KV_API bool SetStrW(const wchar_t* k, const wchar_t* v)
{
	if (!k || !v)
		return false;

	std::lock_guard<std::mutex> _lock(g_mt_strW);
	g_strW[k] = v;
	return true;
}

KV_API const char* GetStrA(const char* k, const char* def /*= ""*/)
{
	if (!k)
		return def;

	std::lock_guard<std::mutex> _lock(g_mt_strA);

	auto itFinder = g_strA.find(k);
	if (itFinder == g_strA.end())
		return def;

	return itFinder->second.c_str();
}

KV_API const wchar_t* GetStrW(const wchar_t* k, const wchar_t* def /*= L""*/)
{
	if (!k)
		return def;

	std::lock_guard<std::mutex> _lock(g_mt_strW);

	auto itFinder = g_strW.find(k);
	if (itFinder == g_strW.end())
		return def;

	return itFinder->second.c_str();
}

KV_API bool HasStrA(const char* k)
{
	if (!k)
		return false;

	std::lock_guard<std::mutex> _lock(g_mt_strA);

	if (g_strA.find(k) == g_strA.end())
		return false;

	return true;
}

KV_API bool HasStrW(const wchar_t* k)
{
	if (!k)
		return false;

	std::lock_guard<std::mutex> _lock(g_mt_strW);

	if (g_strW.find(k) == g_strW.end())
		return false;

	return true;
}

KV_API void DelStrA(const char* k)
{
	if (!k)
		return;

	std::lock_guard<std::mutex> _lock(g_mt_strA);

	g_strA.erase(k);
}

KV_API void DelStrW(const wchar_t* k)
{
	if (!k)
		return;

	std::lock_guard<std::mutex> _lock(g_mt_strW);

	g_strW.erase(k);
}

KV_API bool SetInt(const char* k, int v)
{
	if (!k)
		return false;

	std::lock_guard<std::mutex> _lock(g_mt_int);

	g_int[k] = v;
	return true;
}

KV_API int GetInt(const char* k, int def /*= 0*/)
{
	if (!k)
		return def;

	std::lock_guard<std::mutex> _lock(g_mt_int);

	auto itFinder = g_int.find(k);
	if (itFinder == g_int.end())
		return def;

	return itFinder->second;
}

KV_API bool HasInt(const char* k)
{
	if (!k)
		return false;

	std::lock_guard<std::mutex> _lock(g_mt_int);

	if (g_int.find(k) == g_int.end())
		return false;

	return true;
}

KV_API void DelInt(const char* k)
{
	if (!k)
		return;

	std::lock_guard<std::mutex> _lock(g_mt_int);

	g_int.erase(k);
}

KV_API bool SetDouble(const char* k, double v)
{
	if (!k)
		return false;

	std::lock_guard<std::mutex> _lock(g_mt_double);

	g_double[k] = v;
	return true;
}

KV_API double GetDouble(const char* k, double def /*= 0.0*/)
{
	if (!k)
		return def;

	std::lock_guard<std::mutex> _lock(g_mt_double);

	auto itFinder = g_double.find(k);
	if (itFinder == g_double.end())
		return def;

	return itFinder->second;
}

KV_API bool HasDouble(const char* k)
{
	if (!k)
		return false;

	std::lock_guard<std::mutex> _lock(g_mt_double);

	if (g_double.find(k) == g_double.end())
		return false;

	return true;
}

KV_API void DelDouble(const char* k)
{
	if (!k)
		return;

	std::lock_guard<std::mutex> _lock(g_mt_double);

	g_double.erase(k);
}

KV_API bool SetBuff(const char* k, const char* buff, int buffLen)
{
	if (!k || !buff || buffLen == 0)
		return false;

	std::lock_guard<std::mutex> _lock(g_mt_buff);

	std::vector<char> data(buffLen);
	memcpy(data.data(), buff, buffLen);
	g_buff[k] = data;
	return true;
}

KV_API const char* GetBuff(const char* k, int& outLen)
{
	if (!k)
		return NULL;

	std::lock_guard<std::mutex> _lock(g_mt_buff);

	auto itFinder = g_buff.find(k);
	if (itFinder == g_buff.end())
		return NULL;

	outLen = itFinder->second.size();

	return itFinder->second.data();
}

KV_API bool HasBuff(const char* k)
{
	if (!k)
		return false;

	std::lock_guard<std::mutex> _lock(g_mt_buff);

	if (g_buff.find(k) == g_buff.end())
		return false;

	return true;
}

KV_API void DelBuff(const char* k)
{
	if (!k)
		return;

	std::lock_guard<std::mutex> _lock(g_mt_buff);

	g_buff.erase(k);
}

