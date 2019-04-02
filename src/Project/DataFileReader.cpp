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

JsonParser::JsonParser(const std::string & path):DataFileReader(path) {
}

void JsonParser::traverseJson(IJsonParserCallback * cb) {
	mJsonParserCallback = cb;
	DataFileReader::traverse(this);
	mJsonParserCallback = nullptr;
}

void JsonParser::Callback(const std::string & filename, std::ifstream & file) {
	Json::Value doc = parse(file);
	if (mJsonParserCallback != nullptr)mJsonParserCallback->callback(doc);
}

Json::Value JsonParser::parse(std::ifstream & file) {
	std::string buffer;
	std::string key;
	std::string arg;
	Json::Value doc;
	Json::Value item;
	Json::Value objects;
	TorReaderStatus status = Status_StartFile;
	while (std::getline(file, buffer)) {
		if (buffer.at(0) == '@') {
			status = Status_StartFile;
		} else if (buffer.find("-----BEGIN ") == 0) {
			status = Status_Obj;
			std::string objectBuffer;
			while (std::getline(file, buffer)) {
				if (buffer.find("-----END ") == 0) {
					objects.append(objectBuffer);
					break;
				} else {
					objectBuffer.append(buffer);
				}
			}
		} else {

			if (!key.empty()) {
				if (!arg.empty()) {
					item["arg"] = arg;
				}
				if (!objects.empty()) {
					item["objects"] = objects;
				}
				doc[key].append(item);
			}

			key.clear();
			arg.clear();
			item = Json::Value();
			objects = Json::Value();

			status = Status_Key;
			for (std::string::const_iterator stringIterator = buffer.cbegin(); stringIterator != buffer.cend(); stringIterator++) {
				if (status == Status_Key) {
					if (*stringIterator != ' ' && *stringIterator != '\t') {
						key.push_back(*stringIterator);
					} else {
						status = Status_Arg;

					}
				} else if (status == Status_Arg) {
					arg.push_back(*stringIterator);
				}
			}
			/*if (!key.empty()&&arg.empty()) {
				arg.push_back('1');
			}*/
			status = Status_Obj;
		}
	}
	return doc;
}
