#include "larkxr_adminapi.h"
#include "slice_upload_client.h"


LARK_XR_API lk_adminapi_SliceUploadClient* DC_CALL lk_adminapi_CreateSliceUploadClient()
{
	larkxr_api::SliceUploadClient* client = new larkxr_api::SliceUploadClient();
	return (lk_adminapi_SliceUploadClient*)client;
}

LARK_XR_API void DC_CALL lk_adminapi_ReleaseSliceUploadClient(lk_adminapi_SliceUploadClient* client)
{
	larkxr_api::SliceUploadClient* slice_client = (larkxr_api::SliceUploadClient*)client;
	delete slice_client;
}

LARK_XR_API void DC_CALL lk_adminapi_SliceUpload(lk_adminapi_SliceUploadClient* client, const char* url, const char* path, larkxr_adminapi_SliceUploadCallback callback)
{
	larkxr_api::SliceUploadClient* slice_client = (larkxr_api::SliceUploadClient*)client;
	slice_client->Upload(url, path, [callback](larkxr_api::SliceUploadClient* callback_client) {
		if (callback != nullptr) {
			callback(callback_client->is_success(), callback_client->error_msg().c_str());
		}
	});
}

