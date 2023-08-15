// 
// Created by fcx@pingxingyun.com
// 2023-08-15 15:31
//
#pragma once
#ifndef SLICE_UPLOAD_CLIENT_INCLUDE
#define SLICE_UPLOAD_CLIENT_INCLUDE

#include "utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "hv/requests.h"
#include "larkxr_adminapi.h"

namespace larkxr_api {

// 10M buffer
const uint64_t SLICE_BUFFER_SIZE = 10 * 1024 * 1024;

class LARK_XR_API SliceUploadClient {
public:
	SliceUploadClient();
	~SliceUploadClient();

	/**
	* @param scheme
	* @param host
	* @param port
	* @param adminKey
	* @param adminSecret
	* @param filePath
	*/
	bool Upload(const std::string& scheme, const std::string& host, int port,
		const std::string& adminKey, const std::string& adminSecret,
		const std::string& filePath);

	/**
	* "http://192.168.0.55:8181/appli/getUploadId?adminKey=603284317954637824&timestamp=1692240562355&signature=94111B283E00105C5B2D25F65003B56F3283DAF7";
	* @param scheme 
	* @param host
	* @param port
	* @param adminKey
	* @param timestamp
	* @param signature
	* @param filePath
	* @return success
	*/
	bool Upload(const std::string& scheme, const std::string& host, int port, 
		const std::string& adminKey, const std::string& timestamp, const std::string& signature, 
		const std::string& filePath);

	void GetUpLoadId();

	void SendPost();

	void NextChunk();
private:
	hv::HttpClient http_client_{};
	std::string scheme_ = "";
	std::string host_ = "";
	int port_ = 0;
	hv::QueryParams query_{};

	hv::FormData chunk_{};

	std::ifstream file_stream_{};
	std::string file_name_ = "";
	std::stringstream str_stream_{};
	std::string send_body_ = "";
	uint64_t file_size_ = 0;
	uint64_t current_pos_ = 0;

	std::string upload_id_;
	int chunk_index_ = 0;
	uint64_t chunk_start_ = 0;
	uint64_t chunk_size_ = 0;
	uint64_t total_size_ = 0;
	uint64_t last_size_ = 0;
	bool is_last_chunk_ = 0;
	int total_chunk_cnt_ = 0;
};
}
#endif // SLICE_UPLOAD_CLIENT_INCLUDE