#include "DataFileReader.h"

const std::string DataFileReader::TAG = "DataFileAdaptor";
const std::string DataFileReader::sRootPath = "D:\\WorkSpace\\GraduationDesign\\TorDataProcessing\\Data\\";
const std::string DataFileReader::sServerDescriptorPath = "RelayServerDesc\\server-descriptors-2019-03\\";
const std::string DataFileReader::sExtraInfoPath = "RelayExtraInfoDesc\\extra-infos-2019-03\\";

DataFileReader::DataFileReader(const std::string &path)
{
	mPath = path;
	init();
	mIterator = new DataFileIterator(*this);
}


DataFileReader::~DataFileReader()
{
	delete mIterator;
	mIterator = nullptr;
}

int DataFileReader::listAllFiles(const std::string & path, std::vector<std::string>& list)
{
	size_t pathLen = path.length();
	std::string iPath = path;
	if (iPath.at(pathLen - 1) != '\\') {
		iPath.push_back('\\');
	}

	return listAllFilesInner(iPath, list);
}

int DataFileReader::listAllFilesInner(const std::string & path, std::vector<std::string>& list)
{
	std::string pathRegular = path + "*";
	struct _finddata_t fileInfo = { 0 };
	int fileCount = 0;
	intptr_t fileHandler = _findfirst(pathRegular.c_str(), &fileInfo);
	if (fileHandler != -1) {
		do {
			if ((fileInfo.attrib&_A_SUBDIR)) {
				if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0) {
					std::string newPath = path + fileInfo.name + "\\";
					fileCount += listAllFilesInner(newPath, list);
				}
			}
			else
			{
				fileCount++;
				list.push_back(path + fileInfo.name);
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
		file.open(*iterator, std::ios::in | std::ios::binary);
		if (file.is_open()) {
			callback->Callback(*iterator, file);
			std::cout << TAG << "read file : " << *iterator<<std::endl;
		}
		file.close();
		file.clear();
	}
}

int DataFileReader::getListFileSize() {
	return mFiles.size();
}

DataFileIterator & DataFileReader::begin() {
	if (mIterator != nullptr) {
		delete mIterator;
	}
	mIterator = new DataFileIterator(*this);

	return *mIterator;
}

DataFileIterator & DataFileReader::getIterator() {
	if (mIterator == nullptr) {
		begin();
	}
	return *mIterator;
}

DataFileIterator::DataFileIterator(DataFileReader & reader) :mReader(reader),mVIterator(reader.mFiles.cbegin()){
	mFile = new std::ifstream();
	if (mVIterator != reader.mFiles.cend()) {
		mFile->open(*mVIterator,std::ios::in|std::ios::binary);
	}
}

DataFileIterator::~DataFileIterator() {
	if (mFile != nullptr) {
		if (mFile->is_open()) {
			mFile->close();
			mFile->clear();
		}
		delete mFile;
		mFile = nullptr;
	}
}

std::ifstream &DataFileIterator::get() {
	return *mFile;
}

DataFileIterator &DataFileIterator::next() {
	if (mFile != nullptr) {
		if (mFile->is_open()) {
			mFile->close();
			mFile->clear();
		}
	}

	if (!isEnd()) {
		++mVIterator;
		if (!isEnd()) {
			mFile->open(*mVIterator, std::ios::in | std::ios::binary);
			mFile->bad();
			mFile->fail();
			mFile->eof();
		}
	}
		
	return *this;
}

bool DataFileIterator::isEnd() {
	return mVIterator >= mReader.mFiles.cend();
}

DataFileIterator & DataFileIterator::set(int i) {
	if (mFile != nullptr) {
		if (mFile->is_open()) {
			mFile->close();
			mFile->clear();
		}
	}

	mVIterator = mReader.mFiles.cbegin() + i;
	if (!isEnd()) {
		mFile->open(*mVIterator, std::ios::in | std::ios::binary);
	}

	return *this;
}

std::string DataFileIterator::getFilePath() {
	if (isEnd()) {
		return "";
	}
	return *mVIterator;
}


