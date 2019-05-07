#include "AddrCountTask.h"

AddrCountTask::AddrCountTask() {
}

AddrCountTask::~AddrCountTask() {
}

void AddrCountTask::init() {
}

void AddrCountTask::onParse(const std::string & path, std::ifstream & file, Json::Value & doc) {
	mmdb::Reader& protoReader = mmdb::Reader::getInstance();
}
