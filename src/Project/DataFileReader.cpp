#include "DataFileReader.h"

const std::string DataFileReader::TAG = "DataFileAdaptor";
const std::string DataFileReader::sRootPath = "D:\\WorkSpace\\GraduationDesign\\TorDataProcessing\\Data\\";
const std::string DataFileReader::sServerDescriptorPath = "RelayServerDesc\\server-descriptors-2019-03";
const std::string DataFileReader::sExtraInfoPath = "RelayExtraInfoDesc\\extra-infos-2019-03";

DataFileReader::DataFileReader(const std::string &path)
{
	mCurFile = NULL;
	mPath = path;

	init();
}


DataFileReader::~DataFileReader()
{
	if (mCurFile != NULL) {
		if (mCurFile->is_open()) {
			mCurFile->close();
		}
		delete mCurFile;
		mCurFile = NULL;
	}
}

int DataFileReader::listAllFiles(const std::string & path, std::vector<std::string>& list)
{
	size_t pathLen = path.length();
	std::string iPath;
	if (path.at(pathLen - 1) == '\\') {
		iPath = path.substr(0, pathLen - 1);
	}
	else
	{
		iPath = path;
	}

	return listAllFilesInner(iPath, list);
}

int DataFileReader::listAllFilesInner(const std::string & path, std::vector<std::string>& list)
{
	std::string pathRegular = path + "\\*";
	struct _finddata_t fileInfo = { 0 };
	int fileCount = 0;
	intptr_t fileHandler = _findfirst(pathRegular.c_str(), &fileInfo);
	if (fileHandler != -1) {
		do {
			if ((fileInfo.attrib&_A_SUBDIR)) {
				if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0) {
					std::string newPath = path + "\\" + fileInfo.name;
					fileCount += listAllFilesInner(newPath, list);
				}
			}
			else
			{
				fileCount++;
				list.push_back(path + "\\" + fileInfo.name);
			}
		} while (_findnext(fileHandler, &fileInfo) != -1);
		_findclose(fileHandler);
	}

	return fileCount;
}

int DataFileReader::init()
{
	std::string path = sRootPath + mPath;
	mFiles.clear();
	return listAllFiles(path, mFiles);
}

void DataFileReader::traverse(ITraverseCallback* callback)
{
	if (callback == NULL) {
		return;
	}
	std::vector<std::string>::const_iterator iterator = mFiles.cbegin();
	std::ifstream file;
	for (; iterator != mFiles.cend(); iterator++) {
		file.open(*iterator);
		if (file.is_open()) {
			callback->Callback(*iterator, file);
			std::cout << TAG << "read file : " << *iterator<<std::endl;
		}
		file.close();
	}
}
