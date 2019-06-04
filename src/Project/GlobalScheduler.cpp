
#include "GlobalScheduler.h"


const std::string GlobalScheduler::TAG = "GlobalScheduler";

#define DEBUGTEST

GlobalScheduler::GlobalScheduler() {
}


GlobalScheduler::~GlobalScheduler() {
}

void GlobalScheduler::start() {

	{//system initialize
#ifdef _WIN32
		system("CHCP 65001");	//修改控制台编码为UTF-8
#endif // _WIN32

		Logger::getInstance().init();
	}

#ifdef DEBUGTEST

	test();

#else


	TorDataSpider torDataSpider;
	boost::posix_time::ptime minTime = boost::posix_time::time_from_string("2019-04-01 00:00:00.000");
	torDataSpider.setMinTime(minTime);
	torDataSpider.test();

	TaskManager().run();
#endif
}

void GlobalScheduler::test() {
	MongoConn::getInstance().test();

	//TorDataSpider torDataSpider;
	//boost::posix_time::ptime minTime = boost::posix_time::time_from_string("2019-04-01 00:00:00.000");
	//torDataSpider.setMinTime(minTime);
	//torDataSpider.test();

	TaskManager().run();
}
