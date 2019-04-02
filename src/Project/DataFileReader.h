#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <io.h>
#include <iostream>
#include <jsoncpp/json.h>

class DataFileReader;
class ITraverseCallback;
class JsonParser;
class IJsonParserCallback;

class DataFileReader
{
public:
	const static std::string TAG;
	const static std::string sRootPath;
	const static std::string sServerDescriptorPath;
	const static std::string sExtraInfoPath;

	DataFileReader(const std::string &path);
	~DataFileReader();

	int listAllFiles(const std::string &path, std::vector<std::string> &list);
	
	void traverse(ITraverseCallback* callback);
private:
	int listAllFilesInner(const std::string & path, std::vector<std::string> &list);

	int init();
private:
	std::string mPath;
	std::vector<std::string>::const_iterator mIterator;
	std::ifstream *mCurFile;
	std::vector<std::string> mFiles;
};

class ITraverseCallback {
public:
	virtual void Callback(const std::string & filename, std::ifstream &file) = 0;
};

class DataIterator {
public:
	
};

class JsonParser :public DataFileReader, public ITraverseCallback {
public:
	typedef enum {
		Status_StartFile,
		Status_StartItem,
		Status_Key,
		Status_Arg,
		Status_Obj,
	}TorReaderStatus;

	JsonParser(const std::string &path);

	void traverseJson(IJsonParserCallback *cb);

	virtual void Callback(const std::string & filename, std::ifstream &file) override;

	Json::Value parse(std::ifstream &file);

private:
	IJsonParserCallback *mJsonParserCallback = nullptr;
};

class IJsonParserCallback {
public:
	virtual void callback(Json::Value &json) = 0;
};

