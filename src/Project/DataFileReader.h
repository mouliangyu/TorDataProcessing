#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <io.h>
#include <iostream>
#include <jsoncpp/json.h>

#define DENY_COPY_AND_ASSIGN(clazz) \
	public:\
		clazz(const clazz&) = delete;\
		const clazz &operator=(const clazz &) = delete;

template<class dataType,class IteratorImpl>
class IIterator;
template<class IteratorImpl>
class IIterable;
class DataFileReader;
class DataFileIterator;
class ITraverseCallback;
class DataJsonParser;
class DataJsonIterator;
class IJsonParserCallback;

template<class dataType,class IteratorImpl=IIterator<dataType>>
class IIterator {
public:
	virtual dataType &get() = 0;
	virtual IIterator &next() = 0;
	virtual bool isEnd() = 0;
};

template<class IteratorImpl>
class IIterable {
public:
	virtual IteratorImpl &begin() = 0;
	virtual IteratorImpl &getIterator() = 0;
};

class DataFileReader:public IIterable<DataFileIterator>
{
public:
	friend class DataFileIterator;

	const static std::string TAG;
	const static std::string sRootPath;
	const static std::string sServerDescriptorPath;
	const static std::string sExtraInfoPath;

	DataFileReader(const std::string &path);
	~DataFileReader();

	int listAllFiles(const std::string &path, std::vector<std::string> &list);
	
	void traverse(ITraverseCallback* callback);

	int getListFileSize();

	virtual DataFileIterator &begin()override;
	virtual DataFileIterator &getIterator()override;
private:
	int listAllFilesInner(const std::string & path, std::vector<std::string> &list);

	int init();
private:
	std::string mPath;
	std::vector<std::string> mFiles;
	DataFileIterator *mIterator;
};

class DataFileIterator:public IIterator<std::ifstream,DataFileIterator> {
public:
	DataFileIterator(DataFileReader &reader);
	~DataFileIterator();

	virtual std::ifstream &get() override;
	virtual DataFileIterator &next() override;
	virtual bool isEnd() override;
	DataFileIterator &set(int i);

	std::string getFilePath();

	DENY_COPY_AND_ASSIGN(DataFileIterator)
private:
	DataFileReader &mReader;
	std::vector<std::string>::const_iterator mVIterator;
	std::ifstream *mFile;
};

class ITraverseCallback {
public:
	virtual void Callback(const std::string & filename, std::ifstream &file) = 0;
};

