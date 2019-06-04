
#pragma once

#include <jsoncpp/json.h>
#include <string>
#include <vector>

#include "utils/LogUtils.h"

#define JSONOBJECTPATH(key) "['"##key##"']"
#define JSONARRAYPATH(index) "["###index##"]"

class TorIndexTraverser {

public:
	//listeners

	class JsonTraverseCallback {
	public:
		virtual bool onStartObject(const Json::Value &root, const std::string &path, const std::string &jsonpath) = 0;
		virtual void onEndObject(const Json::Value &root, const std::string &path, const std::string &jsonpath) = 0;
	};

public:

	class DefaultJsonTraverseCallback:public JsonTraverseCallback {
	public:
		//interface TorIndexTraverser::JsonTraverseCallback
		virtual bool onStartObject(const Json::Value &root, const std::string &path, const std::string &jsonpath)override;
		virtual void onEndObject(const Json::Value &root, const std::string &path, const std::string &jsonpath)override;
		static DefaultJsonTraverseCallback &getInstance();
	private :
		static DefaultJsonTraverseCallback *sInstance;
	};


public:
	TorIndexTraverser();
	~TorIndexTraverser();

	void traverse(const Json::Value &root);

	void setJsonTraverseCallback(JsonTraverseCallback *callback);

private:
	void traverseIndex(const Json::Value &root, const std::string &jsonpath, const std::string &path);

private:
	const static std::string TAG;

	JsonTraverseCallback *mJsonTraverseCallback;
};