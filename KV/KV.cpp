// KV.cpp : ���� DLL Ӧ�ó���ĵ���������
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
#include "..\simdb\simdb.hpp"

#define REVERT_KEY "KV.DLL"
#define DES_META "..KV.DLL"//Ҫ��8λ 

std::mutex g_mt_strA;
std::map<std::string, std::string> g_strA;
std::string g_strADef;

std::mutex g_mt_strW;
std::map<std::wstring, std::wstring> g_strW;
std::wstring g_strWDef;

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
std::string g_decryptDataDef;

std::mutex g_mt_encryptData;
std::string g_encryptData;

simdb g_db("KV.DB", 1024, 10);
std::mutex g_mt_sharedData;
std::string g_sharedData;

const char* GetDllPath()
{	
	if (g_dllPath.size() != 0)
		return g_dllPath.c_str();

	//std::lock_guard<std::mutex> _lock(g_mt_dllPath);
	HMODULE hModule = reinterpret_cast<HMODULE>(&__ImageBase);
	char szPathBuffer[MAX_PATH + 1] = { 0 };
	::GetModuleFileNameA(hModule, szPathBuffer, MAX_PATH);
	int len = (int)strlen(szPathBuffer);
	for (int i = len - 1; i > 0; --i) 
	{
		if (szPathBuffer[i] == '\\' || szPathBuffer[i] == '/')
		{
			szPathBuffer[i] = '\0';
			g_dllPath = szPathBuffer;
			break;
		}
	}
	return g_dllPath.c_str();//����DLL����ľ���·��
}

//in out �ĳ��ȱ���Ϊlen
void revertStr(const char* key, int keyLen, const char* in, char* out, int len)
{
	for (int i = 0; i < len; ++i)
	{
		out[i] = key[i%keyLen] ^ in[i];
	}
}

std::string decrypt(const char* str64, int len64, const char* revertKey, const char* desKey)
{
	if (len64 < 1)
		return "";

	int lenDesKey = (int)strlen(desKey);
	char* enkey = new char[lenDesKey];
	revertStr(revertKey, (int)strlen(revertKey), desKey, enkey, lenDesKey);

	std::string s = GL::DES_cbc_decrypt_base64(str64, len64, enkey, lenDesKey, enkey, lenDesKey);
	delete[] enkey;
	return s;
}

std::string encrypt(const char* in, int inlen, const char* revertKey, const char* desKey)
{
	if (inlen < 1)
		return "";

	int lenDesKey = (int)strlen(desKey);
	char* enkey = new char[lenDesKey];
	revertStr(revertKey, (int)strlen(revertKey), desKey, enkey, lenDesKey);

	std::string s = GL::DES_cbc_encrypt_base64(in, inlen, enkey, lenDesKey, enkey, lenDesKey);
	delete[] enkey;

#ifdef _DEBUG
	std::string sDebug = decrypt(s.c_str(), s.length(), revertKey, desKey);
#endif

	return s;
}

KV_API const char* Encrypt(const char* data, const char* desKey)
{
	int datalen = (int)strlen(data);
	if (datalen < 1)
		return "";

	std::lock_guard<std::mutex> _lock(g_mt_encryptData);
	int lenDesKey = (int)strlen(desKey);
	g_encryptData = GL::DES_cbc_encrypt_base64(data, datalen, desKey, lenDesKey, desKey, lenDesKey);
	return g_encryptData.c_str();
}

KV_API const char* Decrypt(const char* data, const char* desKey)
{
	int datalen = (int)strlen(data);
	if (datalen < 1)
		return "";

	std::lock_guard<std::mutex> _lock(g_mt_encryptData);
	int lenDesKey = (int)strlen(desKey);
	g_encryptData = GL::DES_cbc_decrypt_base64(data, datalen, desKey, lenDesKey, desKey, lenDesKey);
	return g_encryptData.c_str();
}

KV_API bool InitEncryData(const char* encryFilePath/* = NULL*/)
{
	std::lock_guard<std::mutex> _lock(g_mt_decryptData);

	g_decryptData.clear();

	std::string sEncryFilePath;
	if (encryFilePath != NULL && strlen(encryFilePath) > 0)
		sEncryFilePath = encryFilePath;
	else
		sEncryFilePath = GetDllPath() + std::string("\\KV.data");
	
	std::fstream fin(sEncryFilePath); //���ļ�
	if (!fin.is_open())
	{
		return false;
	}

	std::string line;
	bool beCrack = false;//���ƽ�
	while (std::getline(fin, line) || beCrack) //���ж�ȡ��ֱ������
	{
		DWORD ts = GetTickCount();//���ƽ�
		if (!beCrack) //���ƽ�
		{
			size_t idx = line.find(':');

			GetTickCount();//���ƽ�

			if (idx == std::string::npos || idx == 0 || idx == line.size() - 1)
				continue;

			GetTickCount();//���ƽ�

			std::string key64 = line.substr(0, idx);

			GetTickCount();//���ƽ�

			std::string val64 = line.substr(idx + 1);

			GetTickCount();//���ƽ�
			if (GetTickCount() - ts > 1000)//���ƽ�
			{
				beCrack = true;//���ƽ�
				continue;//���ƽ�
			}

			std::string key = decrypt(key64.c_str(), (int)key64.length(), REVERT_KEY, DES_META);
			std::string val = decrypt(val64.c_str(), (int)val64.length(), REVERT_KEY, DES_META);
			
			g_decryptData.insert(std::make_pair(key, val));
		}
		else
		{
			g_decryptData.insert(std::make_pair(std::to_string(ts), std::to_string(ts)));//���ƽ�
		}
	}
	fin.close();

	return true;
}

KV_API const char* EncryptData(const char* k, const char* v)
{
	std::string ek = encrypt(k, (int)strlen(k), REVERT_KEY, DES_META);
	std::string ev = encrypt(v, (int)strlen(v), REVERT_KEY, DES_META);

	std::lock_guard<std::mutex> _lock(g_mt_encryptData);
	g_encryptData = ek + ":" + ev;
	return g_encryptData.c_str();
}

KV_API const char* GetDecrypt(const char* k, const char* def /*= ""*/)
{
	if (!k)
		return def;
	
	DWORD ts = GetTickCount();//���ƽ�

	std::lock_guard<std::mutex> _lock(g_mt_decryptData);	
	auto itFinder = g_decryptData.find(k);

	if (GetTickCount() - ts > 1000)//���ƽ�
	{
		g_decryptData.insert(std::make_pair(std::to_string(ts), std::to_string(ts)));//���ƽ�
		return def;//���ƽ�
	}

	if (itFinder == g_decryptData.end())
		return def;

	return itFinder->second.c_str();
}

KV_API int EncryptDataKeysCount()
{
	std::lock_guard<std::mutex> _lock(g_mt_decryptData);
	return (int)g_decryptData.size();
}

KV_API const char* GetEncryptDataKey(int keyIdx)
{
	std::lock_guard<std::mutex> _lock(g_mt_decryptData);
	if ((int)g_decryptData.size() <= keyIdx)
		return "";
	auto it = g_decryptData.begin();
	for (; it != g_decryptData.end() && keyIdx > 0; ++it)
	{
		--keyIdx;
	}
	if (it != g_decryptData.end())
		return it->first.c_str();
	return "";
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
	std::lock_guard<std::mutex> _lock(g_mt_strA);

	if (!k)
	{
		g_strADef = def;
		return g_strADef.c_str();
	}

	auto itFinder = g_strA.find(k);
	if (itFinder == g_strA.end())
	{
		g_strADef = def;
		return g_strADef.c_str();
	}

	return itFinder->second.c_str();
}

KV_API const wchar_t* GetStrW(const wchar_t* k, const wchar_t* def /*= L""*/)
{
	std::lock_guard<std::mutex> _lock(g_mt_strW);

	if (!k)
	{
		g_strWDef = def;
		return g_strWDef.c_str();
	}

	auto itFinder = g_strW.find(k);
	if (itFinder == g_strW.end())
	{
		g_strWDef = def;
		return g_strWDef.c_str();
	}

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

KV_API int StrAKeysCount()
{
	std::lock_guard<std::mutex> _lock(g_mt_strA);
	return (int)g_strA.size();
}

KV_API const char* GetStrAKey(int keyIdx)
{
	std::lock_guard<std::mutex> _lock(g_mt_strA);
	if ((int)g_strA.size() <= keyIdx)
		return "";
	std::map<std::string, std::string>::iterator it = g_strA.begin();
	for (; it != g_strA.end() && keyIdx > 0; ++it)
	{
		--keyIdx;
	}
	if (it != g_strA.end())
		return it->first.c_str();
	return "";
}

KV_API int StrWKeysCount()
{
	std::lock_guard<std::mutex> _lock(g_mt_strW);
	return (int)g_strW.size();
}

KV_API const wchar_t* GetStrWKey(int keyIdx)
{
	std::lock_guard<std::mutex> _lock(g_mt_strW);
	if ((int)g_strW.size() <= keyIdx)
		return L"";
	std::map<std::wstring, std::wstring>::iterator it = g_strW.begin();
	for (; it != g_strW.end() && keyIdx > 0; ++it)
	{
		--keyIdx;
	}
	if (it != g_strW.end())
		return it->first.c_str();
	return L"";
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

KV_API int IntKeysCount()
{
	std::lock_guard<std::mutex> _lock(g_mt_int);
	return (int)g_int.size();
}

KV_API const char* GetIntKey(int keyIdx)
{
	std::lock_guard<std::mutex> _lock(g_mt_int);
	if ((int)g_int.size() <= keyIdx)
		return "";
	std::map<std::string, int>::iterator it = g_int.begin();
	for (; it != g_int.end() && keyIdx > 0; ++it)
	{
		--keyIdx;
	}
	if (it != g_int.end())
		return it->first.c_str();
	return "";
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

KV_API int DoubleKeysCount()
{
	std::lock_guard<std::mutex> _lock(g_mt_double);
	return (int)g_double.size();
}

KV_API const char* GetDoubleKey(int keyIdx)
{
	std::lock_guard<std::mutex> _lock(g_mt_double);
	if ((int)g_double.size() <= keyIdx)
		return "";
	std::map<std::string, double>::iterator it = g_double.begin();
	for (; it != g_double.end() && keyIdx > 0; ++it)
	{
		--keyIdx;
	}
	if (it != g_double.end())
		return it->first.c_str();
	return "";
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

	outLen = (int)(itFinder->second.size());

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

KV_API int BuffKeysCount()
{
	std::lock_guard<std::mutex> _lock(g_mt_buff);
	return (int)g_buff.size();
}

KV_API const char* GetBuffKey(int keyIdx)
{
	std::lock_guard<std::mutex> _lock(g_mt_buff);
	if ((int)g_buff.size() <= keyIdx)
		return "";
	auto it = g_buff.begin();
	for (; it != g_buff.end() && keyIdx > 0; ++it)
	{
		--keyIdx;
	}
	if (it != g_buff.end())
		return it->first.c_str();
	return "";
}

KV_API bool SetSharedMem(const char* k, const char* v)
{
	if (k == NULL)
		return false;
	if (v == NULL)
		return g_db.del(k);
	return (g_db.put(k, v) != 0);
}

KV_API const char* GetSharedMem(const char* k, const char* def /*= ""*/)
{
	if (k == NULL)
	{
		g_sharedData = def;
		return g_sharedData.c_str();
	}

	std::lock_guard<std::mutex> _lock(g_mt_sharedData);
	g_sharedData = g_db.get(k);
	return g_sharedData.c_str();
}

KV_API void DelSharedMem(const char* k)
{
	if (k == NULL)
		return ;
	g_db.del(k);
}


KV_API int SharedMemKeysCount()
{
	return g_db.getKeyStrs().size();
}

KV_API const char* GetSharedMemKey(int keyIdx)
{
	auto keys = g_db.getKeyStrs();

	if ((int)keys.size() <= keyIdx)
		return "";
	
	return keys[keyIdx].str.c_str();
}
