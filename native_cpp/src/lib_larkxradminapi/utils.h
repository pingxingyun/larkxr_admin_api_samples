// 
// Created by fcx@pingxingyun.com
// 2023-08-15 15:31
//
#pragma once
#ifndef UTILS_INCLUDE
#define UTILS_INCLUDE

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stringapiset.h>
#include <time.h>
#include <memory>
#include <vector>
#include <algorithm>
#include "hv/sha1.h"

namespace larkxr_api {
inline static char* Utf8ToGb2312(const char* utf8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
    len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
    if (wstr) delete[] wstr;
    return str;
}
inline static char* Gb2312ToUtf8(const char* gb2312)
{
    int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
    if (wstr) delete[] wstr;
    return str;
}

#ifdef WIN32

typedef struct timeval {
    long tv_sec;
    long tv_usec;
} timeval;


inline static int gettimeofday(struct timeval* tp, struct timezone* tzp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
    // until 00:00:00 January 1, 1970 
    static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    time = ((uint64_t)file_time.dwLowDateTime);
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec = (long)((time - EPOCH) / 10000000L);
    tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
    return 0;
}
#endif // WIN32

inline static uint64_t GetTimestampUs() {
    timeval tv;
    gettimeofday(&tv, nullptr);

    uint64_t Current = (uint64_t)tv.tv_sec * 1000 * 1000 + tv.tv_usec;
    return Current;
}

inline std::string GetTimestampMillStr() {
    uint64_t ms = GetTimestampUs() / 1000;
    return std::to_string(ms);
}

inline static std::string GetSignature(const std::string& appkey, const std::string& secret, const std::string& utc_millseconds)
{

    std::vector<std::string> sort_vector;
    sort_vector.push_back(appkey);
    sort_vector.push_back(secret);
    sort_vector.push_back(utc_millseconds);

    std::sort(sort_vector.begin(), sort_vector.end());

    std::string source;
    for (auto& s : sort_vector)
    {
        source.append(s);
    }

    char output[40];
    hv_sha1_hex((unsigned char*)source.c_str(), source.size(), output, 40);
    return std::string(output);
}
}
#endif // UTILS_INCLUDE