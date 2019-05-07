#pragma once
#include <string>
#include <stack>
#include <jsoncpp/json.h>
#include <maxminddb/maxminddb.h>
#include "utils/LogUtils.h"

namespace mmdb {

	class Reader {
	public:
		const static std::string TAG;
		const static std::string MMDB_FILE;
		static Reader &getInstance();

		Reader();
		~Reader();
		int init();
		int release();
		MMDB_s *getMMDB();
		int lookup(const char *ipstr, MMDB_lookup_result_s *result, int * gai_error, int * MMDB_error);
		int fetch(const char *ipstr, MMDB_entry_data_list_s ** list);
		int lookup_fetch(const char *ipstr, MMDB_lookup_result_s * lookupResult, MMDB_entry_data_list_s ** list);
	private:
		static Reader *sInstance;

		MMDB_s *mMMDB;
	};

	static int OPT_IGNORE_BYTES = 1 << MMDB_DATA_TYPE_BYTES;

	class JsonReader {
	public:
		JsonReader();
		JsonReader(int opt);
		~JsonReader();
		int option;
		int lookup(const char *ipstr, Json::Value &result);
		int parseJson(MMDB_entry_data_list_s ** proto, Json::Value & value);
		int parseJson(MMDB_entry_data_s *proto, Json::Value &value);
	private:
		const static std::string TAG;
	};
}