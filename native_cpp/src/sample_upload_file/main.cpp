// 
// Created by fcx@pingxingyun.com
// 2023-08-11 09:50
//

#include <iostream>
#include <fstream>
#include <sstream>
#include "hv/requests.h"
#include "larkxr_adminapi.h"
#include "slice_upload_client.h"

int main() {
	larkxr_api::SliceUploadClient client;
	// client.Upload("", "E:\\work\\apps\\test.zip");
	// client.Upload("", "E:\\work\\apps\\位型空间-2.zip");
	// adminKey=603284317954637824&timestamp=1692240562355&signature=94111B283E00105C5B2D25F65003B56F3283DAF7
	// client.Upload("http", "192.168.0.55", 8181, "603284317954637824", "1692240562355", "94111B283E00105C5B2D25F65003B56F3283DAF7", "E:\\work\\apps\\位型空间-2.zip");
	// client.Upload("http", "192.168.0.55", 8181, "603284317954637824", "1692240562355", "94111B283E00105C5B2D25F65003B56F3283DAF7", "E:\\work\\apps\\998868509117120513.zip");
	client.Upload("http://192.168.0.55:8181/?adminKey=603284317954637824&timestamp=1692240562355&signature=94111B283E00105C5B2D25F65003B56F3283DAF7", 
		"E:\\work\\apps\\test.zip",
		[](larkxr_api::SliceUploadClient* client) {
			std::cout << "is_success " << client->is_success() << " msg " << client->error_msg() << std::endl;
		});
	// client.Upload("http://192.168.0.55:8181/appli/getUploadId?adminKey=603284317954637824&timestamp=1692240562355&signature=94111B283E00105C5B2D25F65003B56F3283DAF7", "E:\\work\\apps\\位型空间-2.zip");

	larkxr_api::SliceUploadClient* client2 = new larkxr_api::SliceUploadClient();

	client2->Upload("http://192.168.0.55:8181/?adminKey=603284317954637824&timestamp=1692240562355&signature=94111B283E00105C5B2D25F65003B56F3283DAF7",
		"E:\\work\\apps\\test.zip",
		[client2](larkxr_api::SliceUploadClient* client) {
			std::cout << "is_success2 " << client->is_success() << " msg " << client->error_msg() << std::endl;
		});

	delete client2;

	auto client3 = lk_adminapi_CreateSliceUploadClient();
	
	lk_adminapi_SliceUpload(client3,
		"http://192.168.0.55:8181/?adminKey=603284317954637824&timestamp=1692240562355&signature=94111B283E00105C5B2D25F65003B56F3283DAF7",
		"E:\\work\\apps\\test.zip",
		[](int is_success, const char* msg) {
			std::cout << "is_success3 " << is_success << " msg " << msg << std::endl;
		});

	char a;
	std::cin >> a;

	lk_adminapi_ReleaseSliceUploadClient(client3);

    return 0;
}