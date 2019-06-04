#include "AddrCountTask.h"

const std::string AddrCountTask::TAG = "AddrCountTask";
const std::string AddrCountTask::sTitle = "Addr Count of All Relays";
const std::string AddrCountTask::sDescription = "Addr Count of All Relays";

const std::string AddrCountTask::sContinentTitle = "Continent Count of All Relays";
const std::string AddrCountTask::sContinentDescription = "Continent Count of All Relays";

const std::string AddrCountTask::sCountryTitle = "Country Count of All Relays";
const std::string AddrCountTask::sCountryDescription = "Country Count of All Relays";

AddrCountTask::AddrCountTask() {
}

AddrCountTask::AddrCountTask(std::string datePublish) {
	setDatePublish(datePublish);
}

AddrCountTask::~AddrCountTask() {
}

void AddrCountTask::setDatePublish(const std::string & date) {
	boost::regex pattern("[0-9]{4}:[0-9]{2}");
	boost::cmatch mat;
	if (boost::regex_match(date.c_str(), mat, pattern)) {
		mDatePublish = date;
	} else {
		LOGE(TAG, "date pattern error");
	}
	
}

void AddrCountTask::onStart() {
	mTotalCount = 0;
	mInvalidAddrCount = 0;

	mContinentCodeCountMap = Json::Value();
	mCountryCodeCountMap = Json::Value();
	mRegisteredCountryCodeCountMap = Json::Value();
}

void AddrCountTask::onEnd() {

	using bsoncxx::builder::basic::kvp;
	using bsoncxx::builder::basic::make_array;
	using bsoncxx::builder::basic::make_document;
	using bsoncxx::builder::stream::open_document;
	using bsoncxx::builder::stream::close_document;
	using bsoncxx::builder::stream::finalize;

	Json::Value::Members vJsonNames;
	std::vector<std::string>::const_iterator vJsonNameIter;
	int vServerCount;

	auto builder = bsoncxx::builder::stream::document{};
	builder << "title" << sContinentTitle
		<< "description" << sContinentDescription
		<< "date_publish" << mDatePublish;
	vJsonNames = mContinentCodeCountMap.getMemberNames();
	vJsonNameIter = vJsonNames.begin();
	vServerCount = 0;

	builder<<"data" << open_document;
	for (; vJsonNameIter != vJsonNames.end(); vJsonNameIter++) {
		int count= mContinentCodeCountMap[*vJsonNameIter].asInt();
		vServerCount += count;
		builder << *vJsonNameIter << count;
	}
	builder << close_document;

	bsoncxx::document::value vContinentCountDoc = builder << finalize;

	LOGI(TAG, "continent count: %s, total: %d", mContinentCodeCountMap.toStyledString().c_str(), vServerCount);

	builder.clear();
	builder << "title" << sCountryTitle
		<< "description" << sCountryDescription
		<< "date_publish" << mDatePublish;

	vJsonNames = mCountryCodeCountMap.getMemberNames();
	vJsonNameIter = vJsonNames.begin();
	vServerCount = 0;

	builder << "data" << open_document;
	for (; vJsonNameIter != vJsonNames.end(); vJsonNameIter++) {
		int count= mCountryCodeCountMap[*vJsonNameIter].asInt();
		vServerCount += count;
		builder << *vJsonNameIter << count;
	}
	builder << close_document;

	bsoncxx::document::value vCountryCountDoc = builder << finalize;

	LOGI(TAG, "country count: %s, total: %d", mCountryCodeCountMap.toStyledString().c_str(), vServerCount);

	bsoncxx::document::value vAddrCountDoc = make_document(
		kvp("title", sTitle), 
		kvp("description", sDescription), 
		kvp("date_publish", mDatePublish),
		kvp("continent", vContinentCountDoc),
		kvp("country", vCountryCountDoc)
	);

	MongoConn::getInstance().getMeasureDB()->operator[]("alldesc_addrcount").insert_one(vAddrCountDoc.view());



	vJsonNames = mRegisteredCountryCodeCountMap.getMemberNames();
	vJsonNameIter = vJsonNames.begin();
	vServerCount = 0;
	for (; vJsonNameIter != vJsonNames.end(); vJsonNameIter++) {
		vServerCount += mRegisteredCountryCodeCountMap[*vJsonNameIter].asInt();
	}
	LOGI(TAG, "registered country count: %s, total: %d", mRegisteredCountryCodeCountMap.toStyledString().c_str(), vServerCount);

}

void AddrCountTask::onParse(const std::string & path, std::ifstream & file, Json::Value & doc) {
	mTotalCount++;
	mmdb::JsonReader mmdbReader;
	mmdb::Reader& protoReader = mmdb::Reader::getInstance();

	std::string server_router = doc[ServerDescriptorModel::KEY_ROUTER][Json::UInt(0)]["arg"].asString();
	std::vector<std::string> server_router_args;
	XstringUtils::split(server_router, " ", server_router_args);
	std::string server_ip = server_router_args.at(1);
	MMDB_lookup_result_s server_addr_proto = { 0 };
	int status = protoReader.lookup(server_ip.c_str(), &server_addr_proto, NULL, NULL);
	if (status == 0 && server_addr_proto.found_entry) {
		MMDB_entry_data_s server_addr_continent_code = { 0 };
		MMDB_entry_data_s server_addr_country_code = { 0 };
		MMDB_entry_data_s server_addr_registered_country_code = { 0 };

		Json::Value server_addr_continent_code_json;
		Json::Value server_addr_country_code_json;
		Json::Value server_addr_registered_country_iso_code_json;

		MMDB_get_value(&server_addr_proto.entry, &server_addr_continent_code, "continent", "code", NULL);
		MMDB_get_value(&server_addr_proto.entry, &server_addr_country_code, "country", "iso_code", NULL);
		MMDB_get_value(&server_addr_proto.entry, &server_addr_registered_country_code, "registered_country", "iso_code", NULL);

		if (server_addr_continent_code.has_data) {
			mmdbReader.parseJson(&server_addr_continent_code, server_addr_continent_code_json);
			std::string str = server_addr_continent_code_json.asString();
			if (mContinentCodeCountMap.isMember(str)) {
				int count = mContinentCodeCountMap[str].asInt();
				mContinentCodeCountMap[str] = count + 1;
			} else {
				mContinentCodeCountMap[str] = 1;
			}
		} else {
			LOGD(TAG, "server: %s\nip: %s\ndoes not have continent code", path.c_str(), server_ip.c_str());
		}

		if (server_addr_country_code.has_data) {
			mmdbReader.parseJson(&server_addr_country_code, server_addr_country_code_json);
			std::string str = server_addr_country_code_json.asString();
			if (mCountryCodeCountMap.isMember(str)) {
				int count = mCountryCodeCountMap[str].asInt();
				mCountryCodeCountMap[str] = count + 1;
			} else {
				mCountryCodeCountMap[str] = 1;
			}
		} else {
			LOGD(TAG, "server: %s\nip: %s\ndoes not have country code", path.c_str(), server_ip.c_str());
		}

		if (server_addr_registered_country_code.has_data) {
			mmdbReader.parseJson(&server_addr_registered_country_code, server_addr_registered_country_iso_code_json);
			std::string str = server_addr_registered_country_iso_code_json.asString();
			if (mRegisteredCountryCodeCountMap.isMember(str)) {
				int count = mRegisteredCountryCodeCountMap[str].asInt();
				mRegisteredCountryCodeCountMap[str] = count + 1;
			} else {
				mRegisteredCountryCodeCountMap[str] = 1;
			}
		} else {
			LOGD(TAG, "server: %s\nip: %s\ndoes not hava iso code", path.c_str(), server_ip.c_str());
		}

	} else {
		mInvalidAddrCount++;
		LOGD(TAG, "server: %s\nip: %s\nnot found", path.c_str(), server_ip.c_str());
	}

}
