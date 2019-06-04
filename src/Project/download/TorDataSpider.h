#pragma once

#include <string>
#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "curl/curl.h"
#include "utils/LogUtils.h"
#include "jsoncpp/json.h"
#include "utils/TorIndexUtils.h"

class TorDataSpider:public TorIndexTraverser::JsonTraverseCallback {
public:
	class CURL_WriteFunction {
	public:
		static size_t writefunction(void *buffer, size_t _ElementSize, size_t _ElementCount, FILE *_Stream);
		static void reset();
		static void setData(const std::string &data);
		static void getData(std::string &data);
	private:
		static std::string sData;
	};

	class CURL_OstreamCallback {
	public:
		static size_t callback(void *buffer, size_t _ElementSize, size_t _ElementCount, FILE *_Stream);
		static void setOstream(std::ostream &o);
		static size_t getReceiveSize();
		static void reset();
	private:
		static size_t sReceiveSize;
		static std::ostream *sOstream;
	};

public:
	TorDataSpider();
	~TorDataSpider();

	int initCURL();
	int releaseCURL();

	int getLatestIndex(std::string &strIndex, Json::Value &jsonIndex);
	int get(const std::string &url, std::ostream &out);
	int download(const std::string &url, std::string &file);
	void test();

	void setMaxTime(boost::posix_time::ptime &max);
	void setMinTime(boost::posix_time::ptime &min);

public:
	//interface TorIndexTraverser::JsonTraverseCallback
	virtual bool onStartObject(const Json::Value &root, const std::string &path, const std::string &jsonpath)override;
	virtual void onEndObject(const Json::Value &root, const std::string &path, const std::string &jsonpath)override;


private:
	const static std::string TAG;
	const static std::string sDataRootPath;

	bool mCurlInitialized;
	std::map<std::string, std::string> mDownloadFiles;
	std::string mIndexUrlRoot;
	boost::posix_time::ptime mMaxTime;
	boost::posix_time::ptime mMinTime;
	double mTotalSize;
};