#pragma once

#include "persistence/MongoDBConn.h"
#include "TaskManager.h"
#include "download/TorDataSpider.h"

class GlobalScheduler {
public:
	GlobalScheduler();
	~GlobalScheduler();

	void start();

	void test();
private:
	static const std::string TAG;
};

