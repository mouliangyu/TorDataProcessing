#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <io.h>
#include <iostream>

class ITraverseCallback;

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

