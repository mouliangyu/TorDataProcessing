#pragma once

#include "DataJsonReader.h"
#include "tasks/AddrCountTask.h"
#include "tasks/ServerReduceTask.h"
#include "boost/smart_ptr.hpp"

class TaskManager {
public:
	TaskManager();
	~TaskManager();
	void run() {
		DataJsonParser dataJsonParser(DataFileReader::sServerDescriptorPath);

		boost::shared_ptr<IJsonParserCallback> addrCountTask(new AddrCountTask("2019-03"));
		dataJsonParser.registerCallback(addrCountTask);

		boost::shared_ptr<IJsonParserCallback> serverReduceTask(new ServerReduceTask());
		dataJsonParser.registerCallback(serverReduceTask);

		dataJsonParser.run();
	}
};

