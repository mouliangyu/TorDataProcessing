
#pragma once

#include <chrono>
#include <string>

#include <boost/smart_ptr.hpp>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/array.hpp>

#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/logger.hpp>

#include <utils/LogUtils.h>

class MongoConn {
public:

	MongoConn();
	~MongoConn();

	static MongoConn &getInstance();

	int init();

	int release();

	int test();

	boost::shared_ptr<mongocxx::database> getMeasureDB();

private:

	class MongoLogger;

	const static std::string TAG;
	const static std::string sDefaultUri;

	static MongoConn *sInstance;

	boost::shared_ptr<mongocxx::instance> mMongoInstance;
	boost::shared_ptr<mongocxx::client> mMongoClient;
	boost::shared_ptr<mongocxx::database> mMongoDB;
};