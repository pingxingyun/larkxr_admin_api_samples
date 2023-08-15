// 
// Created by fcx@pingxingyun.com
// 2023-08-11 09:50
//

#include <iostream>
#include <fstream>
#include <sstream>
#include "hv/requests.h"
#include "slice_upload_client.h"

int main() {
	larkxr_api::SliceUploadClient client;
	// client.Upload("", "E:\\work\\apps\\test.zip");
	// client.Upload("", "E:\\work\\apps\\位型空间-2.zip");
	// adminKey=603284317954637824&timestamp=1692240562355&signature=94111B283E00105C5B2D25F65003B56F3283DAF7
	// client.Upload("http", "192.168.0.55", 8181, "603284317954637824", "1692240562355", "94111B283E00105C5B2D25F65003B56F3283DAF7", "E:\\work\\apps\\位型空间-2.zip");
	client.Upload("http", "192.168.0.55", 8181, "603284317954637824", "1692240562355", "94111B283E00105C5B2D25F65003B56F3283DAF7", "E:\\work\\apps\\998868509117120513.zip");


	char a;
	std::cin >> a;
    return 0;
}