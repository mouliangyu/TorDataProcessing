#pragma once

#include "AbsDataTask.h"
#include "utils/mmdb/Reader.h"

class AddrCountTask :public AbsDataTask {
public:
	AddrCountTask();
	~AddrCountTask();

	virtual void init() override;
	virtual void onParse(const std::string &path, std::ifstream &file, Json::Value &doc) override;
private:
	Json::Value vAddrContinentCodeCountMap;
	Json::Value vAddrCountryCodeCountMap;
	Json::Value vAddrRegisterCountryISOCodeCountMap;
};