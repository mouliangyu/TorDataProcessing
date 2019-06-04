#pragma once
#include "DataFileReader.h"
#include "boost/smart_ptr.hpp"

class DataJsonParser :public ITraverseCallback, public IIterable<DataJsonIterator> {
public:
	friend class DataJsonIterator;

	typedef enum {
		Status_StartFile,
		Status_StartItem,
		Status_Key,
		Status_Arg,
		Status_Obj,
	}TorReaderStatus;

	DataJsonParser(const std::string &path);
	~DataJsonParser();

	void registerCallback(boost::shared_ptr<IJsonParserCallback> &callback);
	void unregisterCallback(boost::shared_ptr<IJsonParserCallback> &callback);

	void run();

	int getFileListSize();

	DataJsonIterator &begin();
	DataJsonIterator &getIterator();

	virtual void Callback(const std::string & filename, std::ifstream &file) override;

	Json::Value parse(std::ifstream &file,Json::Value &doc);

private:
	DataFileReader *mDataFileReader;
	IJsonParserCallback *mJsonParserCallback;
	std::vector<boost::weak_ptr<IJsonParserCallback>> mJsonCallbacklist;
	DataJsonIterator *mIterator;
};

class DataJsonIterator :public IIterator<Json::Value, DataJsonIterator> {
public:
	DataJsonIterator(DataJsonParser &jsonParser);
	~DataJsonIterator();

	virtual Json::Value &get() override;
	virtual DataJsonIterator &next() override;
	virtual bool isEnd() override;
	DataJsonIterator &set(int i);

	std::ifstream &getFile();
	std::string getFilePath();

	DENY_COPY_AND_ASSIGN(DataJsonIterator)
private:
	DataJsonParser &mJsonParser;
	DataFileIterator &mDataFileIterator;
	Json::Value mDoc;
};

class IJsonParserCallback {
public:
	virtual void onStart() = 0;
	virtual void onEnd() = 0;
	virtual void onParse(const std::string &path,std::ifstream &file,Json::Value &doc) = 0;
};

