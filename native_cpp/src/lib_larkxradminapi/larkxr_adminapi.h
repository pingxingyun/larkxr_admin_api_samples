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

#define DC_CALL __stdcall

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct lk_adminapi_SliceUploadClient;
typedef void(*larkxr_adminapi_SliceUploadCallback)(int is_success, const char* msg);

LARK_XR_API lk_adminapi_SliceUploadClient* DC_CALL lk_adminapi_CreateSliceUploadClient();

LARK_XR_API void DC_CALL lk_adminapi_ReleaseSliceUploadClient(lk_adminapi_SliceUploadClient* client);

LARK_XR_API void DC_CALL lk_adminapi_SliceUpload(lk_adminapi_SliceUploadClient* client, const char* url, const char* path, larkxr_adminapi_SliceUploadCallback callback);

#if defined( __cplusplus )
}
#endif

#endif // LARKXR_ADMINAPI_INCLUDE