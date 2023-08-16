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
const uint64_t LARK_XR_API_SUCCESS_CODE = 1000;
const int FAILED_RETRY_TIME = 3;

class LARK_XR_API SliceUploadClient {
public:
	typedef std::function<void(SliceUploadClient*)> SliceUploadClientCallback;

	SliceUploadClient();
	~SliceUploadClient();



	/**
	* @param url http://192.168.0.55:8181?adminKey=603284317954637824&timestamp=1692240562355&signature=94111B283E00105C5B2D25F65003B56F3283DAF7
	* @param filePath
	*/
	bool Upload(const std::string& url, const std::string& filePath, SliceUploadClientCallback callback = nullptr);

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
		const std::string& filePath,
		SliceUploadClientCallback callback = nullptr);

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
		const std::string& filePath,
		SliceUploadClientCallback callback = nullptr);

	void GetUpLoadId();

	void SendPost();

	void NextChunk();

	void Finish(bool success);

	inline void SetCallback(SliceUploadClientCallback callback) {
		callback_ = callback;
	}

	bool is_success() const { return is_success_; };
	bool processing() const { return processing_; };
	bool finish() const { return finish_; };

	const std::string& upload_id() const { return upload_id_; };
	int chunk_index() { return chunk_index_; };
	uint64_t chunk_start() const { return chunk_start_; };
	uint64_t chunk_size() const { return chunk_size_; };
	uint64_t total_size() const { return total_size_; };
	uint64_t last_size() const { return last_size_; };
	bool is_last_chunk() const { return is_last_chunk_; };
	int total_chunk_cnt() const { return total_chunk_cnt_; };

	const std::string& error_msg() { return error_msg_; }
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
	
	bool is_success_ = false;
	bool processing_ = false;
	bool finish_ = false;
	int current_retry_ = 0;

	std::string error_msg_ = "";

	SliceUploadClientCallback callback_ = nullptr;
};
}
#endif // SLICE_UPLOAD_CLIENT_INCLUDE