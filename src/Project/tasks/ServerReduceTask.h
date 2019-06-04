#pragma once

#include <jsoncpp/json.h>
#include <boost/unordered_map.hpp>
#include "AbsDataTask.h"
#include "ServerDescriptorModel.h"
#include "utils/XstringUtils.h"
#include "utils/LogUtils.h"

typedef std::vector<std::string> stringlist;
typedef boost::unordered_map<std::string, stringlist> umap;

class ServerReduceTask : public AbsDataTask{
public:
	ServerReduceTask();
	~ServerReduceTask();

	virtual void onStart() override;
	virtual void onEnd() override;
	virtual void onParse(const std::string &path, std::ifstream &file, Json::Value &doc) override;

private:
	const static std::string TAG;

	umap vFingerprintHashMap;
};