#include "DataJsonReader.h"

DataJsonParser::DataJsonParser(const std::string & path) :mJsonParserCallback(nullptr), mIterator(nullptr) {
	mDataFileReader = new DataFileReader(path);
}

DataJsonParser::~DataJsonParser() {
	delete mDataFileReader;
}

void DataJsonParser::registerCallback(boost::shared_ptr<IJsonParserCallback>& callback) {
	mJsonCallbacklist.push_back(callback);
}

void DataJsonParser::unregisterCallback(boost::shared_ptr<IJsonParserCallback>& callback) {
	size_t count = mJsonCallbacklist.size();
	for (size_t i = 0; i < count; i++) {
		if (!mJsonCallbacklist[i].expired()) {
			boost::shared_ptr<IJsonParserCallback> ref = mJsonCallbacklist[i].lock();
			if (ref.get() == callback.get()) {
				std::swap(mJsonCallbacklist[i], mJsonCallbacklist[count - 1]);
				mJsonCallbacklist.pop_back();
				return;
			}
		}
	}
}

void DataJsonParser::run() {
	size_t count = mJsonCallbacklist.size();
	for (int i = 0; i < count; i++) {
		if (!mJsonCallbacklist[i].expired()) {
			mJsonCallbacklist[i].lock()->onStart();
		}
	}

	mDataFileReader->traverse(this);

	for (int i = 0; i < count; i++) {
		if (!mJsonCallbacklist[i].expired()) {
			mJsonCallbacklist[i].lock()->onEnd();
		}
	}
}

int DataJsonParser::getFileListSize() {
	return mDataFileReader->getListFileSize();
}

DataJsonIterator & DataJsonParser::begin() {
	if (mIterator != nullptr) {
		delete mIterator;
	}
	mIterator = new DataJsonIterator(*this);

	return *mIterator;
}

DataJsonIterator & DataJsonParser::getIterator() {
	if (mIterator == nullptr) {
		begin();
	}
	return *mIterator;
}

void DataJsonParser::Callback(const std::string & filename, std::ifstream & file) {
	Json::Value doc;
	parse(file, doc);
	size_t count = mJsonCallbacklist.size();
	for (int i = 0; i < count; i++) {
		if (!mJsonCallbacklist[i].expired()) {
			mJsonCallbacklist[i].lock()->onParse(filename, file, doc);
		}
	}
}

Json::Value DataJsonParser::parse(std::ifstream & file,Json::Value &doc) {
	std::string buffer;
	std::string key;
	std::string arg;
	Json::Value item;
	Json::Value objects;
	TorReaderStatus status = Status_StartFile;
	doc = Json::Value();
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

	//clear file status;
	file.clear();

	return doc;
}

DataJsonIterator::DataJsonIterator(DataJsonParser & jsonParser) :mJsonParser(jsonParser), mDataFileIterator(jsonParser.mDataFileReader->begin()) {
	std::ifstream &file = mDataFileIterator.get();
	if (file.is_open()) {
		mJsonParser.parse(file,mDoc);
	}
}

DataJsonIterator::~DataJsonIterator() {
}

Json::Value & DataJsonIterator::get() {
	return mDoc;
}

DataJsonIterator& DataJsonIterator::next() {
	mDataFileIterator.next();
	std::ifstream &file = mDataFileIterator.get();
	if (file.is_open()) {
		mJsonParser.parse(file,mDoc);
	} else {
		mDoc.clear();
	}
	return *this;
}

bool DataJsonIterator::isEnd() {
	return mDataFileIterator.isEnd();
}

DataJsonIterator & DataJsonIterator::set(int i) {
	return *this;
}

std::ifstream & DataJsonIterator::getFile() {
	return mDataFileIterator.get();
}

std::string DataJsonIterator::getFilePath() {
	return mDataFileIterator.getFilePath();
}