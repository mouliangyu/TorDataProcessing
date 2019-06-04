#pragma once

#include <boost/regex.hpp>
#include <bsoncxx/builder/stream/document.hpp>

#include "AbsDataTask.h"
#include "ServerDescriptorModel.h"
#include "persistence/MongoDBConn.h"

#include "utils/mmdb/Reader.h"
#include "utils/XstringUtils.h"
#include "utils/LogUtils.h"


class AddrCountTask :public AbsDataTask {
public:
	AddrCountTask();
	AddrCountTask(std::string datePublish);
	~AddrCountTask();

	void setDatePublish(const std::string &date);
	virtual void onStart() override;
	virtual void onEnd() override;
	virtual void onParse(const std::string &path, std::ifstream &file, Json::Value &doc) override;
private:
	const static std::string TAG;

	const static std::string sTitle;
	const static std::string sDescription;

	const static std::string sContinentTitle;
	const static std::string sContinentDescription;

	const static std::string sCountryTitle;
	const static std::string sCountryDescription;

	int mTotalCount;
	int mInvalidAddrCount;

	std::string mDatePublish;

	Json::Value mContinentCodeCountMap;
	Json::Value mCountryCodeCountMap;
	Json::Value mRegisteredCountryCodeCountMap;
};