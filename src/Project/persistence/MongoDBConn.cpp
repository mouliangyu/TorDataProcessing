#include "MongoDBConn.h"

const std::string MongoConn::TAG = "MongoConn";
const std::string MongoConn::sDefaultUri = "mongodb://127.0.0.1:27017";

MongoConn *MongoConn::sInstance = nullptr;

MongoConn::MongoConn() {
	init();
}

MongoConn::~MongoConn() {
	release();
}

MongoConn & MongoConn::getInstance() {
	if (sInstance == nullptr)sInstance = new MongoConn();
	return *sInstance;
}

int MongoConn::init() {

	mongocxx::uri vUri = mongocxx::uri{ sDefaultUri };
	
	mMongoInstance = boost::make_shared<mongocxx::instance>(std::make_unique<MongoLogger>());

	mMongoClient = boost::make_shared<mongocxx::client>(vUri);

	mMongoDB = boost::make_shared<mongocxx::database>((*mMongoClient)["tor_collector"]);

	return 0;
}

int MongoConn::release() {

	mMongoInstance.reset();

	return 0;
}

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;
// Construct a document in the format of 'messagelist'.
bsoncxx::document::value new_message(int64_t uid, int32_t status, std::string msg) {
	return make_document(kvp("uid", uid), kvp("status", status), kvp("msg", msg));
}

// Insert a document into the database.
void insert_test_data(mongocxx::collection& coll) {
	bsoncxx::document::value doc =
		make_document(kvp("messagelist",
			make_array(new_message(413098706, 3, "Lorem ipsum..."),
				new_message(413098707, 2, "Lorem ipsum..."),
				new_message(413098708, 1, "Lorem ipsum..."))));

	// Normally, one should check the return value for success.
	coll.insert_one(std::move(doc));
}

int MongoConn::test() {

	using bsoncxx::builder::basic::kvp;
	using bsoncxx::builder::basic::make_array;
	using bsoncxx::builder::basic::make_document;

	bsoncxx::document::value doc = make_document(kvp("testName","basicTest"));

	mongocxx::collection coll = (*mMongoClient)["test"]["events"];
	coll.drop();
	insert_test_data(coll);

	return 0;
}

boost::shared_ptr<mongocxx::database> MongoConn::getMeasureDB() {
	return boost::make_shared<mongocxx::database>((*mMongoClient)["tor_measurement"]);
}

class MongoConn::MongoLogger:public mongocxx::logger {
public:
	virtual void operator()(mongocxx::log_level level,
		boost::string_view domain,
		boost::string_view message) noexcept override {

		severity_level mylevel = severity_level::info;
		switch (level) {
		case mongocxx::v_noabi::log_level::k_error:
			mylevel = severity_level::fatal;
			break;
		case mongocxx::v_noabi::log_level::k_critical:
			mylevel = severity_level::error;
			break;
		case mongocxx::v_noabi::log_level::k_warning:
			mylevel = severity_level::warning;
			break;
		case mongocxx::v_noabi::log_level::k_message:
			break;
		case mongocxx::v_noabi::log_level::k_info:
			break;
		case mongocxx::v_noabi::log_level::k_debug:
			mylevel = severity_level::debug;
			break;
		case mongocxx::v_noabi::log_level::k_trace:
			break;
		default:
			break;
		};

		Logger::getInstance().log(mylevel,domain.to_string(),message.to_string());
	}
};
