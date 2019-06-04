
#include "TorDataSpider.h"
#include "boost/thread/thread_pool.hpp"

const std::string TorDataSpider::TAG = "TorDataSpider";
const std::string TorDataSpider::sDataRootPath = "D:\\WorkSpace\\GraduationDesign\\TorDataProcessing\\LocalRepo\\TorCollector";

TorDataSpider::TorDataSpider()
	:mCurlInitialized(false)
	,mTotalSize(0.0f)
	,mMaxTime(boost::posix_time::second_clock::local_time())
	,mMinTime(boost::posix_time::from_time_t(0))
{
	initCURL();
	boost::filesystem::path dataRootPath(sDataRootPath);
	boost::filesystem::create_directories(dataRootPath);
}

TorDataSpider::~TorDataSpider() {
	releaseCURL();
}

int TorDataSpider::initCURL() {
	if (!mCurlInitialized) {
		curl_global_init(CURL_GLOBAL_ALL);
		mCurlInitialized = true;
	}
	return 0;
}

int TorDataSpider::releaseCURL() {
	if (!mCurlInitialized) {
		curl_global_cleanup();
		mCurlInitialized = false;
	}
	return 0;
}

int TorDataSpider::getLatestIndex(std::string & strIndex, Json::Value & jsonIndex) {
	LOGD(TAG, "start");
	CURL *curl = NULL;
	CURLcode vRet = CURLE_OK;

	if (!mCurlInitialized) {
		LOGD(TAG, "curl must be initialized!");
		return -1;
	}

	curl = curl_easy_init();

	if (curl == NULL) {
		LOGE(TAG, "curl_easy_init() fail!");
		return -1;
	} else {
		//make request header
		struct curl_slist *header = NULL;
		curl_slist_append(header, "Connection: Keep-Alive");
		curl_slist_append(header, "Accept-Language: zh-Hans-CN, zh-Hans; q=0.8, en-US; q=0.5, en; q=0.3");
		curl_slist_append(header, "Cache-Control: max-age=0");
		curl_slist_append(header, "Accept: text/html, application/xhtml+xml, application/xml; q=0.9, */*; q=0.8");
		curl_slist_append(header, "Host: metrics.torproject.org");
		curl_slist_append(header, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36 Edge/18.17763");
		curl_slist_append(header, "DNT: 1");
		curl_slist_append(header, "Cache-Control: max-age=0");
		curl_slist_append(header, "Accept-Encoding: gzip, deflate, br");

		curl_easy_setopt(curl, CURLOPT_URL, "https://collector.torproject.org/index/index.json");
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
		//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		//use proxy over firewall
		curl_easy_setopt(curl, CURLOPT_PROXY, "127.0.0.1:1080");
		curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5_HOSTNAME);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CURL_WriteFunction::writefunction);

		CURL_WriteFunction::reset();
		vRet = curl_easy_perform(curl);

		//fetch index
		strIndex = std::string();
		jsonIndex = Json::Value();
		CURL_WriteFunction::getData(strIndex);
		Json::Reader jsonReader;
		bool b = jsonReader.parse(strIndex, jsonIndex, false);

		if (vRet != CURLE_OK) {
			LOGE(TAG, "curl_easy_perform error: %s", curl_easy_strerror(vRet));
		}

		curl_slist_free_all(header);

	}

	curl_easy_cleanup(curl);

	return 0;
}

int TorDataSpider::get(const std::string & url, std::ostream & out) {

	LOGD(TAG, "start");
	CURL *curl = NULL;
	CURLcode vRet = CURLE_OK;

	if (!mCurlInitialized) {
		LOGD(TAG, "curl must be initialized!");
		return -1;
	}

	curl = curl_easy_init();

	if (curl == NULL) {
		LOGE(TAG, "curl_easy_init() fail!");
		return -1;
	} else {
		//make request header
		struct curl_slist *header = NULL;
		curl_slist_append(header, "Connection: Keep-Alive");
		curl_slist_append(header, "Accept-Language: zh-Hans-CN, zh-Hans; q=0.8, en-US; q=0.5, en; q=0.3");
		curl_slist_append(header, "Cache-Control: max-age=0");
		curl_slist_append(header, "Accept: text/html, application/xhtml+xml, application/xml; q=0.9, */*; q=0.8");
		//curl_slist_append(header, "Host: metrics.torproject.org");
		curl_slist_append(header, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36 Edge/18.17763");
		curl_slist_append(header, "DNT: 1");
		curl_slist_append(header, "Cache-Control: max-age=0");
		curl_slist_append(header, "Accept-Encoding: deflate, br");

		curl_easy_setopt(curl, CURLOPT_URL, "https://collector.torproject.org/index/index.json");
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
		//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		//use proxy over firewall
		curl_easy_setopt(curl, CURLOPT_PROXY, "127.0.0.1:1080");
		curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5_HOSTNAME);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CURL_OstreamCallback::callback);
		CURL_OstreamCallback::reset();
		CURL_OstreamCallback::setOstream(out);

		//curl_easy_setopt(curl,CURLOPT_FILE,)

		vRet = curl_easy_perform(curl);

		if (vRet != CURLE_OK) {
			LOGE(TAG, "curl_easy_perform error: %s", curl_easy_strerror(vRet));
		} 

		curl_slist_free_all(header);

	}

	curl_easy_cleanup(curl);

	return CURL_OstreamCallback::getReceiveSize();
}

int TorDataSpider::download(const std::string & url, std::string & file) {
	LOGD(TAG, "start");
	CURL *curl = NULL;
	CURLcode vRet = CURLE_OK;

	if (!mCurlInitialized) {
		LOGD(TAG, "curl must be initialized!");
		return -1;
	}

	curl = curl_easy_init();

	if (curl == NULL) {
		LOGE(TAG, "curl_easy_init() fail!");
		return -1;
	} else {
		//make request header
		struct curl_slist *header = NULL;
		curl_slist_append(header, "Connection: Keep-Alive");
		curl_slist_append(header, "Accept-Language: zh-Hans-CN, zh-Hans; q=0.8, en-US; q=0.5, en; q=0.3");
		/*curl_slist_append(header, "Cache-Control: max-age=0");*/
		curl_slist_append(header, "Accept: text/html, application/xhtml+xml, application/xml; q=0.9, */*; q=0.8");
		//curl_slist_append(header, "Host: metrics.torproject.org");
		curl_slist_append(header, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36 Edge/18.17763");
		curl_slist_append(header, "DNT: 1");
		curl_slist_append(header, "Accept-Encoding: gzip, deflate, br");

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION,CURL_OstreamCallback::callback);
		CURL_OstreamCallback::reset();
		std::ostringstream strStream;
		CURL_OstreamCallback::setOstream(strStream);
		//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		curl_easy_setopt(curl, CURLOPT_POST, 0L);
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

		//use proxy over firewall
		curl_easy_setopt(curl, CURLOPT_PROXY, "127.0.0.1:1080");
		curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5_HOSTNAME);

		FILE *fo = fopen(file.c_str(), "wb");
		curl_easy_setopt(curl, CURLOPT_FILE, fo);

		vRet = curl_easy_perform(curl);

		fclose(fo);

		LOGD(TAG, "response header: %s", strStream.str().c_str());

		if (vRet != CURLE_OK) {
			LOGE(TAG, "curl_easy_perform error: %s", curl_easy_strerror(vRet));
		}

		curl_slist_free_all(header);

	}

	curl_easy_cleanup(curl);

	return 0;
}

void TorDataSpider::test() {

	using namespace std;
	using namespace Json;

	string vStrIndex;
	Value vJsonIndex;

	initCURL();
	getLatestIndex(vStrIndex, vJsonIndex);


	TorIndexTraverser torIndexTraverser;

	mIndexUrlRoot = vJsonIndex["path"].asString();
	mTotalSize = 0.0f;
	mDownloadFiles.clear();
	torIndexTraverser.setJsonTraverseCallback(this);
	torIndexTraverser.traverse(vJsonIndex);
	LOGD(TAG, "total size: %.2f", mTotalSize);

	for (std::map<std::string,std::string>::iterator iter=mDownloadFiles.begin(); iter!=mDownloadFiles.end(); iter++) {
		std::string url = iter->first;
		std::string localpathstr = iter->second;
		boost::filesystem::path localpath(localpathstr);
		if (!boost::filesystem::exists(localpath)) {
			/*boost::filesystem::ofstream outfile(localpath,ios::out|ios::binary);
			int receiveSize=get(url, outfile);
			LOGD(TAG, "receive count: %d", receiveSize);
			outfile.close();*/
			download(url, localpathstr);
		}
	}
}

void TorDataSpider::setMaxTime(boost::posix_time::ptime & max) {
	mMaxTime = max;
}

void TorDataSpider::setMinTime(boost::posix_time::ptime & min) {
	mMinTime = min;
}

bool TorDataSpider::onStartObject(const Json::Value & root, const std::string & path, const std::string & jsonpath) {
	std::string vPath;
	if (root.isMember("path")) {
		vPath = root["path"].asString();

		//ignore recent data
		if (vPath == "recent") {
			return false;
		}
		if (path.length()!=0) {
			std::string relativePath = path.substr(mIndexUrlRoot.length());
			boost::filesystem::path localPath((sDataRootPath + relativePath));
			boost::filesystem::create_directory(localPath);
		}
	}

	if (root.isMember("size")) {
		//file
		std::string fileurl = (path+'/').append(vPath);
		std::string localpath = std::string(sDataRootPath).append(path.substr(mIndexUrlRoot.length())).append("/").append(vPath);

		LOGI(TAG, "file url: %s", fileurl.c_str());

		mTotalSize += root["size"].asUInt();

		std::string lastmodifiedstr = root["last_modified"].asString();

		boost::posix_time::ptime lastmodified = boost::posix_time::time_from_string(lastmodifiedstr);
		
		if (lastmodified <= mMaxTime && lastmodified >= mMinTime) {
			//collect file to download
			mDownloadFiles.insert(std::make_pair(fileurl, localpath));
		}

	}

	return true;
}

void TorDataSpider::onEndObject(const Json::Value & root, const std::string & path, const std::string & jsonpath) {
}

std::string TorDataSpider::CURL_WriteFunction::sData = "";

size_t TorDataSpider::CURL_WriteFunction::writefunction(void * buffer, size_t _ElementSize, size_t _ElementCount, FILE * _Stream) {
	size_t byteCount = _ElementSize * _ElementCount;

	sData.append((char*)buffer, byteCount);

	return _ElementCount;
}

void TorDataSpider::CURL_WriteFunction::reset() {
	sData = "";
}

void TorDataSpider::CURL_WriteFunction::setData(const std::string & data) {
	sData = data;
}

void TorDataSpider::CURL_WriteFunction::getData(std::string & data) {
	data = sData;
}

std::ostream *TorDataSpider::CURL_OstreamCallback::sOstream = NULL;
size_t TorDataSpider::CURL_OstreamCallback::sReceiveSize = 0;

size_t TorDataSpider::CURL_OstreamCallback::callback(void * buffer, size_t _ElementSize, size_t _ElementCount, FILE * _Stream) {

	size_t byteCount = _ElementCount * _ElementSize;
	sReceiveSize += byteCount;

	sOstream->write((char*)buffer, byteCount);

	return _ElementCount;
}

void TorDataSpider::CURL_OstreamCallback::setOstream(std::ostream & o) {
	sOstream = &o;
}

size_t TorDataSpider::CURL_OstreamCallback::getReceiveSize() {
	return sReceiveSize;
}

void TorDataSpider::CURL_OstreamCallback::reset() {
	sOstream = NULL;
	sReceiveSize = 0;
}
