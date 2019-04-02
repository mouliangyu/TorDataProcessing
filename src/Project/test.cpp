#include "DataFileReader.h"
#include <jsoncpp/json.h>

#define BUF_MAXLEN 1024

typedef enum {
	Status_StartFile,
	Status_StartItem,
	Status_Key,
	Status_Arg,
	Status_Obj,
}TorReaderStatus;

class ServerDescriptorCallback :public ITraverseCallback {
public:
	void Callback(const std::string & filename, std::ifstream &file) override {
		std::string buffer;
		std::string key;
		std::string arg;
		std::vector<std::string> objectBuffer;
		TorReaderStatus status=Status_StartFile;
		while (std::getline(file, buffer)) {
			if (buffer.at(0) == '@') {
				status = Status_StartFile;
			}
			else {
				key.clear();
				arg.clear();
				status = Status_Key;
				for (std::string::const_iterator stringIterator = buffer.cbegin(); stringIterator != buffer.cend(); stringIterator++) {
					if (status == Status_Key) {
						if (*stringIterator != ' ' || *stringIterator != '\t') {
							key.push_back(*stringIterator);
						}
						else {
							status = Status_Arg;
						}
					}
					else if (status == Status_Arg) {
						arg.push_back(*stringIterator);
					}
				}
				status = Status_Obj;
			}
		}
	}
};

int main(int argc, char* argv[]) {
	DataFileReader serverDescriptor(DataFileReader::sServerDescriptorPath);
	ServerDescriptorCallback serverDescCb;
	serverDescriptor.traverse(&serverDescCb);

	return 0;
}