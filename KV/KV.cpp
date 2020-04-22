// KV.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "KV.h"
#include <map>
#include <string>
#include <vector>
#include <mutex>

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

