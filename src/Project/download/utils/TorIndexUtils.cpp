
#include "TorIndexUtils.h"

const std::string TorIndexTraverser::TAG = "TorIndexTraverser";
TorIndexTraverser::DefaultJsonTraverseCallback *TorIndexTraverser::DefaultJsonTraverseCallback::sInstance = NULL;

TorIndexTraverser::TorIndexTraverser()
	:mJsonTraverseCallback(NULL)
{
	setJsonTraverseCallback(&DefaultJsonTraverseCallback::getInstance());
}

TorIndexTraverser::~TorIndexTraverser(){
	mJsonTraverseCallback = NULL;
}

void TorIndexTraverser::traverse(const Json::Value & root) {
	traverseIndex(root, "", "");
}

void TorIndexTraverser::setJsonTraverseCallback(JsonTraverseCallback * callback) {
	mJsonTraverseCallback = callback;
}

void TorIndexTraverser::traverseIndex(const Json::Value & root, const std::string &jsonpath, const std::string &path) {
	if (mJsonTraverseCallback != NULL) {
		if (mJsonTraverseCallback->onStartObject(root, path, jsonpath)) {

			//TODO


			//to child
			std::string extendPath = path;

			if (root.type() == Json::ValueType::objectValue) {

				std::string vPath = root["path"].asString();
				if (vPath.size() > 0) {
					if (extendPath.length() > 0) {
						extendPath.push_back('/');
					}
					extendPath.append(vPath);
				}

				if (root.isMember("files")) {
					std::string vJsonPath = jsonpath + JSONOBJECTPATH("files");

					const Json::Value &files = root["files"];
					Json::UInt i = 0;
					Json::UInt size = files.size();
					for (; i < size; i++) {
						std::string vvJsonPath = vJsonPath + JSONARRAYPATH(i);

						const Json::Value &file = files[i];
						traverseIndex(file, vvJsonPath, extendPath);
					}
				}

				if (root.isMember("directories")) {
					std::string vJsonPath = jsonpath + JSONOBJECTPATH("directories");

					const Json::Value &directories = root["directories"];
					Json::UInt i = 0;
					Json::UInt size = directories.size();
					for (; i < size; i++) {
						std::string vvJsonPath = vJsonPath + JSONARRAYPATH(i);

						const Json::Value &directory = directories[i];
						traverseIndex(directory, vvJsonPath, extendPath);
					}
				}
			}
	
		} else {
			LOGD(TAG, "canceled by onStartObject()");
		}

		if (mJsonTraverseCallback != NULL)mJsonTraverseCallback->onEndObject(root, path, jsonpath);
	}

	
}

bool TorIndexTraverser::DefaultJsonTraverseCallback::onStartObject(const Json::Value & root, const std::string & path, const std::string & jsonpath) {
	return true;
}

void TorIndexTraverser::DefaultJsonTraverseCallback::onEndObject(const Json::Value & root, const std::string & path, const std::string & jsonpath) {
}

TorIndexTraverser::DefaultJsonTraverseCallback & TorIndexTraverser::DefaultJsonTraverseCallback::getInstance() {
	if (sInstance == NULL) {
		sInstance = new TorIndexTraverser::DefaultJsonTraverseCallback();
	}
	return *sInstance;
}
