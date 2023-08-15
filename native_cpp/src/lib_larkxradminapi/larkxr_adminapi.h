// 
// Created by fcx@pingxingyun.com
// 2023-08-15 15:31
//
#pragma once
#ifndef LARKXR_ADMINAPI_INCLUDE
#define LARKXR_ADMINAPI_INCLUDE

#if defined(_WIN32) && defined(LARK_XR_DLL)
#if defined(LARK_XR_EXPORT)
		#define LARK_XR_API __declspec(dllexport)
	#else
		#define LARK_XR_API __declspec(dllimport)
	#endif
#endif

#if !defined(LARK_XR_API)
#if defined (__GNUC__) && (__GNUC__ >= 4)
#define LARK_XR_API __attribute__ ((visibility ("default")))
#else
#define LARK_XR_API
#endif
#endif

#endif // LARKXR_ADMINAPI_INCLUDE