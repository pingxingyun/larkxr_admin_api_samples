// 
// Created by fcx@pingxingyun.com
// 2023-08-15 15:31
//
#include "slice_upload_client.h"
#include "hv/hpath.h"

namespace larkxr_api {
const std::string API_PATH_GET_UPLOAD_ID = "/appli/getUploadId";
const std::string API_PATH_SLICE_UPLOAD = "/appli/sliceUpload";

SliceUploadClient::SliceUploadClient() {
}

SliceUploadClient::~SliceUploadClient() {
	if (file_stream_.is_open()) {
		file_stream_.close();
	}
}

bool SliceUploadClient::Upload(const std::string& url, const std::string& filePath, SliceUploadClientCallback callback)
{
	HttpRequestPtr req(new HttpRequest);
	req->url = url;
	req->ParseUrl();
	int res = hv::parse_query_params(url.c_str(), req->query_params);
	return res != -1 && Upload(req->scheme, req->host, req->port, req->query_params["adminKey"], req->query_params["timestamp"], req->query_params["signature"], filePath, callback);
}

bool SliceUploadClient::Upload(const std::string& scheme, const std::string& host, int port, const std::string& adminKey, const std::string& adminSecret, const std::string& filePath, SliceUploadClientCallback callback)
{
	std::string timestamp = GetTimestampMillStr();
	std::string signature = GetSignature(adminKey, adminSecret, timestamp);
	return Upload(scheme, host, port, adminKey, timestamp, signature, filePath, callback);
}

bool SliceUploadClient::Upload(const std::string& scheme, const std::string& host, int port, const std::string& adminKey, const std::string& timestamp, const std::string& signature, const std::string& filePath, SliceUploadClientCallback callback) {
	callback_ = callback;
	
	file_stream_.open(filePath, std::ios::binary | std::ios::in);

	if (!file_stream_.is_open()) {
		std::cout << "open file failed." << std::endl;
		error_msg_ = "open file failed.";

		if (callback_ != nullptr) {
			callback_(this);
		}
		return false;
	}

	scheme_ = scheme;
	host_ = host;
	port_ = port;
	query_["adminKey"] = adminKey;
	query_["timestamp"] = timestamp;
	query_["signature"] = signature;

	uint64_t begain = file_stream_.tellg();
	file_stream_.seekg(0, std::ios::end);
	file_size_ = file_stream_.tellg();
	file_stream_.seekg(begain, std::ios::beg);

	current_pos_ = file_stream_.tellg();

	std::cout << "file size " << file_size_ << " current " << current_pos_ << " " << begain << std::endl;

	file_name_ = HPath::basename(filePath);

	upload_id_ = "";
	chunk_index_ = 0;
	chunk_start_ = current_pos_;
	chunk_size_ = SLICE_BUFFER_SIZE;
	total_size_ = file_size_;
	total_chunk_cnt_ = total_size_ / chunk_size_ + 1;
	last_size_ = total_size_ - chunk_index_ * chunk_size_;
	is_last_chunk_ = last_size_ <= SLICE_BUFFER_SIZE;

	chunk_.filename = file_name_;
	chunk_.content.resize(SLICE_BUFFER_SIZE);
	// SendPost(http_client_);

	processing_ = true;
	error_msg_ = "";

	GetUpLoadId();
	return true;
}

void SliceUploadClient::GetUpLoadId() {
	HttpRequestPtr req(new HttpRequest);
	req->method = HTTP_GET;
	// req->url = "http://192.168.0.55:8181/appli/getUploadId?adminKey=603284317954637824&timestamp=1692240562355&signature=94111B283E00105C5B2D25F65003B56F3283DAF7";
	req->headers["Connection"] = "keep-alive";
	req->scheme = scheme_;
	req->host = host_;
	req->port = port_;
	req->query_params = query_;
	req->path = API_PATH_GET_UPLOAD_ID;
	req->DumpUrl();

	std::cout << "send url " << req->url << std::endl;

	http_client_.sendAsync(req, [this](const HttpResponsePtr& resp) {
		if (resp == NULL) {
			printf("request failed!\n");
			error_msg_ = "get upload id failed.";
			is_success_ = false;
			Finish(false);
			return;
		}
		else {
			printf("%d %s\r\n", resp->status_code, resp->status_message());
			printf("%s\n", Utf8ToGb2312(resp->body.c_str()));
		}

		hv::Json json;
		std::string err;
		int res = hv::parse_json(resp->body.c_str(), json, err);
		if (res != 0) {
			printf("parse json failed %d  %s \n", res, err.c_str());
			error_msg_ = "get upload id failed. resp body parse failed.";
			Finish(false);
			return;
		}

		int code = json["code"].get<int>();

		if (code != LARK_XR_API_SUCCESS_CODE) {
			error_msg_ = "get upload id failed.";
			Finish(false);
			return;
		}

		upload_id_ = json["result"];

		printf("get upload id %s \n", upload_id_.c_str());

		std::cout << "send first " << chunk_index_ << std::endl;

		this->SendPost();
	});
}

void SliceUploadClient::SendPost() {
	HttpRequestPtr req(new HttpRequest);
	req->method = HTTP_POST;
	// req->url = "http://192.168.0.55:8181/appli/sliceUpload?adminKey=603284317954637824&timestamp=1692240562355&signature=94111B283E00105C5B2D25F65003B56F3283DAF7";
	req->scheme = scheme_;
	req->host = host_;
	req->port = port_;
	req->query_params = query_;
	req->path = API_PATH_SLICE_UPLOAD;
	req->DumpUrl();

	std::cout << "send url " << req->url << std::endl;

	req->headers["Connection"] = "keep-alive";
	// req->headers["Content-Type"] = "keep-alive";
	// req->body = "This is an async request.";
	req->timeout = 10;
	req->form["uploadId"] = upload_id_;
	req->form["chunkIndex"] = chunk_index_;
	req->form["chunkStart"] = chunk_start_;
	req->form["chunkSize"] = is_last_chunk_ ? last_size_ : chunk_size_;
	req->form["totalChunkCnt"] = total_chunk_cnt_;
	req->form["originalFilename"] = file_name_;

	if (is_last_chunk_) {
		chunk_.content.resize(last_size_);
	}
	file_stream_.read((char*)chunk_.content.c_str(), chunk_.content.size());

	req->form["chunk"] = chunk_;

	http_client_.sendAsync(req, [this](const HttpResponsePtr& resp) {
		if (resp == NULL) {
			printf("request failed!\n");
			if (current_retry_ < FAILED_RETRY_TIME) {
				SendPost();
				return;
			}
			Finish(false);
			return;
		}
		printf("%d %s\r\n", resp->status_code, resp->status_message());
		printf("%s\n", Utf8ToGb2312(resp->body.c_str()));

		hv::Json json;
		std::string err;
		int res = hv::parse_json(resp->body.c_str(), json, err);
		if (res != 0) {
			printf("parse json failed %d  %s \n", res, err.c_str());
			if (current_retry_ < FAILED_RETRY_TIME) {
				SendPost();
				return;
			}
			Finish(false);
			return;
		}

		int code = json["code"].get<int>();

		if (code != LARK_XR_API_SUCCESS_CODE) {
			if (current_retry_ < FAILED_RETRY_TIME) {
				SendPost();
				return;
			}
			Finish(false);
			return;
		}

		current_retry_ = 0;

		if (!is_last_chunk_) {
			this->NextChunk();
		}
		else {
			std::cout << "send success" << std::endl;
			Finish(true);
		}
	});
}

void SliceUploadClient::NextChunk() {
	chunk_index_++;
	file_stream_.seekg(SLICE_BUFFER_SIZE * chunk_index_, std::ios::beg);
	current_pos_ = file_stream_.tellg();
	chunk_start_ = current_pos_;
	last_size_ = total_size_ - chunk_index_ * chunk_size_;
	is_last_chunk_ = last_size_ <= SLICE_BUFFER_SIZE;

	std::cout << "send next " << chunk_index_ << std::endl;

	SendPost();
}
void SliceUploadClient::Finish(bool success)
{
	is_success_ = success;
	finish_ = true;
	processing_ = false;
	current_retry_ = 0;
	chunk_index_ = 0;
	current_pos_ = 0;
	last_size_ = 0;
	chunk_start_ = 0;
	is_last_chunk_ = false;
	if (file_stream_.is_open()) {
		file_stream_.close();
	}
	if (callback_) {
		callback_(this);
	}
}
}