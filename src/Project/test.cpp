#include "DataFileReader.h"

class ServerDescriptorCallback:public IJsonParserCallback{
public:
	virtual void callback(Json::Value &json) override {

	};
};

int main(int argc, char* argv[]) {
	DataFileReader serverDescriptor(DataFileReader::sServerDescriptorPath);
	ServerDescriptorCallback serverDescCb;

	return 0;
}