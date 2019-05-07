
#include "Reader.h"

using namespace mmdb;

const std::string mmdb::Reader::TAG = "mmdb::Reader";
const std::string mmdb::Reader::MMDB_FILE = "D:\\WorkSpace\\GraduationDesign\\TorDataProcessing\\Data\\GeoLite2-Country1.mmdb";
Reader *mmdb::Reader::sInstance = NULL;

Reader & mmdb::Reader::getInstance() {
	if (sInstance == NULL)sInstance = new Reader();
	return *sInstance;
}

mmdb::Reader::Reader() {
	init();
}

mmdb::Reader::~Reader() {
	release();
}

int mmdb::Reader::init() {
	mMMDB = (MMDB_s *)malloc(sizeof(MMDB_s));
	if (mMMDB == NULL) {
		LOGE(TAG, "cannot allocate memory for MMDB");
		return -1;
	}
	int status = MMDB_open(MMDB_FILE.c_str(), MMDB_MODE_MMAP, mMMDB);
	if (status != MMDB_SUCCESS) {
		LOGE(TAG, "open db error: %s", MMDB_strerror(status));
	}
	return status;
}

int mmdb::Reader::release() {
	LOGD(TAG, "release()");
	if (mMMDB != NULL) {
		free(mMMDB);
	}
	return 0;
}

MMDB_s * mmdb::Reader::getMMDB() {
	return mMMDB;
}

int mmdb::Reader::lookup(const char * ipstr, MMDB_lookup_result_s * result, int * gai_error, int * MMDB_error) {
	int tgai_error, tMMDB_error;
	if (result != NULL && ipstr != NULL) {
		*result = MMDB_lookup_string(mMMDB, ipstr, &tgai_error, &tMMDB_error);
	}
	if (gai_error != NULL)*gai_error = tgai_error;
	if (MMDB_error != NULL)*MMDB_error = tMMDB_error;
	return (tgai_error == 0 && tMMDB_error == 0) ? 0 : -1;
}

int mmdb::Reader::fetch(const char * ipstr, MMDB_entry_data_list_s ** list) {
	int gai_error, mmdb_error, vRet = 0;
	MMDB_lookup_result_s lookupResult = MMDB_lookup_string(mMMDB, ipstr, &gai_error, &mmdb_error);
	if (gai_error != 0) {
		LOGE(TAG, "gai_error: %s", gai_strerror(gai_error));
		vRet = gai_error;
	}
	if (mmdb_error != 0) {
		LOGE(TAG, "mmdb_error: %s", MMDB_strerror(mmdb_error));
		vRet = mmdb_error;
	}
	if (!lookupResult.found_entry) {
		LOGE(TAG, "cannot find ip: %s", ipstr);
		vRet = -1;
	}
	if (vRet == 0 && list != NULL) {
		vRet = MMDB_get_entry_data_list(&lookupResult.entry, list);
	}

	return vRet;
}

int mmdb::Reader::lookup_fetch(const char * ipstr, MMDB_lookup_result_s * lookupResult, MMDB_entry_data_list_s ** list) {
	if (ipstr == NULL) {
		LOGE(TAG, "lookup_fetch(): ipstr cannot be null");
		return -1;
	}
	if (lookupResult == NULL) {
		LOGE(TAG, "lookup_fetch(): lookupResult cannot be null");
		return -1;
	}
	int gai_error, mmdb_error, vRet = 0;
	*lookupResult = MMDB_lookup_string(mMMDB, ipstr, &gai_error, &mmdb_error);
	if (gai_error != 0) {
		LOGE(TAG, "gai_error: %s", gai_strerror(gai_error));
		vRet = gai_error;
	}
	if (mmdb_error != 0) {
		LOGE(TAG, "mmdb_error: %s", MMDB_strerror(mmdb_error));
		vRet = mmdb_error;
	}
	if (!lookupResult->found_entry) {
		LOGE(TAG, "cannot find ip: %s", ipstr);
		vRet = -1;
	}
	if (vRet == 0&&list!=NULL) {
		vRet = MMDB_get_entry_data_list(&lookupResult->entry, list);
	}
	
	return vRet;
}

const std::string mmdb::JsonReader::TAG = "mmdb::JsonReader";

mmdb::JsonReader::JsonReader() {
	option = OPT_IGNORE_BYTES;
}

mmdb::JsonReader::JsonReader(int opt) {
	option = opt;
}

mmdb::JsonReader::~JsonReader() {
}

int mmdb::JsonReader::lookup(const char * ipstr, Json::Value & result) {
	MMDB_entry_data_list_s *entry;
	int vRet = Reader::getInstance().fetch(ipstr, &entry );
	if (vRet == 0) {
		if (parseJson(&entry, result) != 0) {
			vRet = -1;
		} 
	}
	return vRet;
}

int mmdb::JsonReader::parseJson(MMDB_entry_data_list_s ** proto, Json::Value & value) {
	if (proto == NULL||*proto==NULL) {
		LOGE(TAG, "parseJson: MMDB_entry_data_list_s pointer cannot be null");
		return -1;
	}

	MMDB_entry_data_s &curEntryData = (*proto)->entry_data;
	std::stringstream strbuf;
	switch ((*proto)->entry_data.type) {
	case MMDB_DATA_TYPE_UTF8_STRING:	//2
		value = std::string(curEntryData.utf8_string, curEntryData.data_size);
		break;
	case MMDB_DATA_TYPE_DOUBLE:			//3
		value = curEntryData.double_value;
		break;
	case MMDB_DATA_TYPE_BYTES:			//4
	{
		if (option&OPT_IGNORE_BYTES == OPT_IGNORE_BYTES) {
			value = "__ignored_bytes__";
		}
		break;
	}
	case MMDB_DATA_TYPE_UINT16:			//5
		value = curEntryData.uint16;
		break;
	case MMDB_DATA_TYPE_UINT32:			//6
		value = curEntryData.uint32;
		break;
	case MMDB_DATA_TYPE_INT32:			//8
		value = curEntryData.int32;
		break;
	case MMDB_DATA_TYPE_UINT64:			//9
		strbuf.str("");
		strbuf << "0x" << std::hex << curEntryData.uint64;
		value = strbuf.str();
		break;
	case MMDB_DATA_TYPE_UINT128:		//10
	{
		strbuf.str("");
		uint8_t *uint128_ptr = curEntryData.uint128;
		strbuf << "0x";
		for (int i = 0; i < 16; i++) {
			strbuf << std::hex << uint128_ptr[i];
		}
		value = strbuf.str();
		break;
	}
	case MMDB_DATA_TYPE_BOOLEAN:		//14
		value = curEntryData.boolean;
		break;
	case MMDB_DATA_TYPE_FLOAT:			//15
		value = curEntryData.float_value;
		break;
	case MMDB_DATA_TYPE_MAP:			//7
	{
		value = Json::Value(Json::ValueType::objectValue);
		int dataSize = (*proto)->entry_data.data_size;
		
		for (int i = 0; i < dataSize; i++) {
			std::string mapKey;
			Json::Value mapValue;

			*proto = (*proto)->next;
			if (*proto != NULL && (*proto)->entry_data.type == MMDB_DATA_TYPE_UTF8_STRING) {
				mapKey = std::string((*proto)->entry_data.utf8_string, (*proto)->entry_data.data_size);
			} else {
				LOGE(TAG, "parseJson: invalid key");
				return -1;
			}

			*proto = (*proto)->next;
			if (parseJson(proto, mapValue) == 0) {
				value[mapKey] = mapValue;
			} else {
				LOGE(TAG, "parseJson: invalid value");
				return -1;
			}
		}
		break;
	}
	case MMDB_DATA_TYPE_ARRAY:			//11
	{
		value = Json::Value(Json::ValueType::arrayValue);
	
		int dataSize = (*proto)->entry_data.data_size;
		
		for (int i = 0; i < dataSize; i++) {
			*proto = (*proto)->next;
			Json::Value arrayValue;
			if (parseJson(proto, arrayValue) == 0) {
				value.append(arrayValue);
			} else {
				LOGE(TAG, "parseJson: invalid array value");
				return -1;
			}
		}
	}
	default:
		LOGE(TAG, "lookup: invalid value type: %d", curEntryData.type);
		return -1;
	}
	return 0;
}

int mmdb::JsonReader::parseJson(MMDB_entry_data_s * proto, Json::Value & value) {
	MMDB_entry_data_s &curEntryData = *proto;
	std::stringstream strbuf;
	switch (curEntryData.type) {
	case MMDB_DATA_TYPE_UTF8_STRING:	//2
		value = std::string(curEntryData.utf8_string, curEntryData.data_size);
		break;
	case MMDB_DATA_TYPE_DOUBLE:			//3
		value = curEntryData.double_value;
		break;
	case MMDB_DATA_TYPE_BYTES:			//4
	{
		if (option&OPT_IGNORE_BYTES == OPT_IGNORE_BYTES) {
			value = "__ignored_bytes__";
		} else {
			value = "__not_implement_bytes__";
		}
		break;
	}
	case MMDB_DATA_TYPE_UINT16:			//5
		value = curEntryData.uint16;
		break;
	case MMDB_DATA_TYPE_UINT32:			//6
		value = curEntryData.uint32;
		break;
	case MMDB_DATA_TYPE_INT32:			//8
		value = curEntryData.int32;
		break;
	case MMDB_DATA_TYPE_UINT64:			//9
		strbuf.str("");
		strbuf << "0x" << std::hex << curEntryData.uint64;
		value = strbuf.str();
		break;
	case MMDB_DATA_TYPE_UINT128:		//10
	{
		strbuf.str("");
		uint8_t *uint128_ptr = curEntryData.uint128;
		strbuf << "0x";
		for (int i = 0; i < 16; i++) {
			strbuf << std::hex << uint128_ptr[i];
		}
		value = strbuf.str();
		break;
	}
	case MMDB_DATA_TYPE_BOOLEAN:		//14
		value = curEntryData.boolean;
		break;
	case MMDB_DATA_TYPE_FLOAT:			//15
		value = curEntryData.float_value;
		break;
	case MMDB_DATA_TYPE_MAP:			//7
	case MMDB_DATA_TYPE_ARRAY:			//11
		LOGE(TAG, "lookup: entry data is not a leaf node");
	default:
		LOGE(TAG, "lookup: invalid value type: %d", curEntryData.type);
		return -1;
	}
	return 0;
}
