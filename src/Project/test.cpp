
#include <direct.h>
#include <stdio.h>
#include <mpi.h>
#include <boost/filesystem.hpp>
#include <boost/log/common.hpp>
#include <boost/unordered_map.hpp>
#include "utils/XstringUtils.h"
#include "utils/XfileUtils.h"
#include "DataJsonReader.h"
#include "ServerDescriptorModel.h"
#include "utils/LogUtils.h"
#include "maxminddb/maxminddb.h"
#include "utils/mmdb/Reader.h"


const static std::string KEY_FINGERPRINT = "fingerprint";
const static std::string MMDB_FILE = "D:\\WorkSpace\\GraduationDesign\\TorDataProcessing\\Data\\GeoLite2-Country.mmdb";
const static Json::UInt INDEX_MAIN_FINGERPRINT = 0;
const static std::string TAG = "main";

const char *ip = "220.158.216.150";
const char *ip1 = "123.112.15.171";

typedef std::vector<std::string> stringlist;
typedef boost::unordered_map<std::string, stringlist> umap;

int main(int argc, char* argv[]) {
	system("CHCP 65001");	//修改控制台编码为UTF-8

	/*MPI_Init(&argc, &argv);
	int commSize, commRank;
	MPI_Comm_size(MPI_COMM_WORLD, &commSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &commRank);*/

	Json::Value addrDoc;
	mmdb::JsonReader mmdbReader;
	mmdbReader.lookup(ip1, addrDoc);
	LOGD(TAG, addrDoc.toStyledString().c_str());

	mmdb::Reader &protoReader = mmdb::Reader::getInstance();
	MMDB_entry_data_list_s *proto;
	MMDB_lookup_result_s lookupResult;
	MMDB_entry_data_s entry_data;
	protoReader.lookup(ip, &lookupResult, NULL, NULL);
	int status = MMDB_get_value(&lookupResult.entry, &entry_data, "registered_country","iso_code", NULL);
	Json::Value tValue;
	mmdbReader.parseJson(&entry_data, tValue);
	LOGD(TAG, MMDB_strerror(status));
	LOGD(TAG, tValue.asCString());

	std::string outRootPath = DataFileReader::sRootPath + "_DataOut\\";
	std::string outRootPathName = outRootPath;
	outRootPathName.erase(outRootPathName.length() - 1, 1);
	boost::filesystem::path bOutRootPath(outRootPathName);
	boost::filesystem::remove_all(bOutRootPath);

	std::string vServerOutPath = outRootPath + "RelayServer\\";

	int inPathServerDescLen = DataFileReader::sRootPath.length() + DataFileReader::sServerDescriptorPath.length();
	int inPathExtraInfoLen = DataFileReader::sRootPath.length() + DataFileReader::sExtraInfoPath.length();

	DataJsonParser serverDesc(DataFileReader::sServerDescriptorPath);
	DataJsonParser extraInfo(DataFileReader::sExtraInfoPath);

	umap vFingerprintHashMap;
	vFingerprintHashMap.clear();

	Json::Value vAddrContinentCodeCountMap;
	Json::Value vAddrCountryCodeCountMap;
	Json::Value vAddrRegisterCountryISOCodeCountMap;

	//std::ofstream outFile;

	//int size = serverDesc.getFileListSize()/commSize;
	//int start = size * commRank;
	//if (commRank == commSize - 1)size = serverDesc.getFileListSize();
	DataJsonIterator &serverDescIterator = serverDesc.begin();
	int index = 1;
	int totalCount = 0;
	int invalidAddrCount = 0;
	for (; !serverDescIterator.isEnd(); serverDescIterator.next(),index++) {
		totalCount++;
		Json::Value &serverDescDoc = serverDescIterator.get();

		std::string inFilePathServerDesc = serverDescIterator.getFilePath();

		std::string inFileRelativePathServerDesc = inFilePathServerDesc;
		inFileRelativePathServerDesc.erase(0, inPathServerDescLen);

		std::string outFilePath = vServerOutPath + inFileRelativePathServerDesc;

		std::string outFilePathParentServerDesc = outFilePath;
		std::string::const_reverse_iterator vPathStrIteratorServerDesc = outFilePathParentServerDesc.crbegin();
		int inFileNameLenServerDesc = 0;
		while (1) {
			if (*vPathStrIteratorServerDesc == '\\' || *vPathStrIteratorServerDesc == '/')break;
			++vPathStrIteratorServerDesc;
			++inFileNameLenServerDesc;
		}
		outFilePathParentServerDesc.erase(outFilePathParentServerDesc.length() - inFileNameLenServerDesc - 1, inFileNameLenServerDesc + 1);

		boost::filesystem::path bOutFilePathParentServerDesc(outFilePathParentServerDesc);
		boost::filesystem::create_directories(bOutFilePathParentServerDesc);

		//outFile.open(outFilePath,std::ios::out|std::ios::binary);
		//outFile << inFilePathServerDesc << std::endl;
		//std::ifstream &inFileServerDesc = serverDescIterator.getFile();
		//inFileServerDesc.seekg(0);
		//XfileUtils::copyFile(inFileServerDesc, outFile);

		std::string server_bandwidth = serverDescDoc[ServerDescriptorModel::KEY_BANDWIDTH][Json::UInt(0)]["arg"].asString();
		std::string server_router = serverDescDoc[ServerDescriptorModel::KEY_ROUTER][Json::UInt(0)]["arg"].asString();
		std::vector<std::string> server_router_args;
		XstringUtils::split(server_router, " ", server_router_args);
		std::string server_ip = server_router_args.at(1);
		MMDB_lookup_result_s server_addr_proto = { 0 };
		int status=protoReader.lookup(server_ip.c_str(), &server_addr_proto, NULL, NULL);
		if (status == 0 && server_addr_proto.found_entry) {
			MMDB_entry_data_s server_addr_continent_code = { 0 };
			MMDB_entry_data_s server_addr_country_code = { 0 };
			MMDB_entry_data_s server_addr_registered_country_iso_code = { 0 };

			Json::Value server_addr_continent_code_json;
			Json::Value server_addr_country_code_json;
			Json::Value server_addr_registered_country_iso_code_json;

			MMDB_get_value(&server_addr_proto.entry, &server_addr_continent_code, "continent", "code", NULL);
			MMDB_get_value(&server_addr_proto.entry, &server_addr_country_code, "country", "iso_code", NULL);
			MMDB_get_value(&server_addr_proto.entry, &server_addr_registered_country_iso_code, "registered_country", "iso_code", NULL);

			if (server_addr_continent_code.has_data) {
				mmdbReader.parseJson(&server_addr_continent_code, server_addr_continent_code_json);
				std::string str = server_addr_continent_code_json.asString();
				if (vAddrContinentCodeCountMap.isMember(str)) {
					int count = vAddrContinentCodeCountMap[str].asInt();
					vAddrContinentCodeCountMap[str] = count + 1;
				} else {
					vAddrContinentCodeCountMap[str] = 1;
				}
			} else {
				LOGD(TAG, "server: %s\nip: %s\ndoes not have continent code",inFilePathServerDesc.c_str(),server_ip.c_str());
			}

			if (server_addr_country_code.has_data) {
				mmdbReader.parseJson(&server_addr_country_code, server_addr_country_code_json);
				std::string str = server_addr_country_code_json.asString();
				if (vAddrCountryCodeCountMap.isMember(str)) {
					int count = vAddrCountryCodeCountMap[str].asInt();
					vAddrCountryCodeCountMap[str] = count + 1;
				} else {
					vAddrCountryCodeCountMap[str] = 1;
				}
			} else {
				LOGD(TAG, "server: %s\nip: %s\ndoes not have country code", inFilePathServerDesc.c_str(), server_ip.c_str());
			}

			if (server_addr_registered_country_iso_code.has_data) {
				mmdbReader.parseJson(&server_addr_registered_country_iso_code, server_addr_registered_country_iso_code_json);
				std::string str = server_addr_registered_country_iso_code_json.asString();
				if (vAddrRegisterCountryISOCodeCountMap.isMember(str)) {
					int count = vAddrRegisterCountryISOCodeCountMap[str].asInt();
					vAddrRegisterCountryISOCodeCountMap[str] = count + 1;
				} else {
					vAddrRegisterCountryISOCodeCountMap[str] = 1;
				}
			} else {
				LOGD(TAG, "server: %s\nip: %s\ndoes not hava iso code", inFilePathServerDesc.c_str(), server_ip.c_str());
			}

		} else {
			invalidAddrCount++;
			LOGD(TAG, "server: %s\nip: %s\nnot found", inFilePathServerDesc.c_str(), server_ip.c_str());
		}

		

		//LOGI(TAG, "server: %s\nip: %s\naddress detail: %s", inFilePathServerDesc.c_str(),server_ip.c_str(), server_addrDoc.toStyledString().c_str());

		//if (server_addrDoc.isMember("code")) {
		//	std::string addrCode = server_addrDoc["code"].asString();
		//	if (vAddrCodeCountMap.isMember(addrCode)) {
		//		int count = vAddrCodeCountMap[addrCode].asInt();
		//		vAddrCodeCountMap[addrCode] = count + 1;
		//	} else {
		//		vAddrCodeCountMap[addrCode] = 1;
		//	}
		//} else {
		//	LOGW(TAG, "cannot find address : %s", inFilePathServerDesc.c_str());
		//}

		//if (server_addrDoc.isMember("names")) {
		//	Json::Value addrNames = server_addrDoc["names"];
		//	if (addrNames.isMember("en")) {
		//		std::string addrNameEN = addrNames["en"].asString();
		//		if (vAddrNameCountMap_EN.isMember(addrNameEN)) {
		//			int count = vAddrNameCountMap_EN[addrNameEN].asInt();
		//			vAddrNameCountMap_EN[addrNameEN] = count + 1;
		//		} else {
		//			vAddrNameCountMap_EN[addrNameEN] = 1;
		//		}
		//	} else {
		//		LOGW(TAG, "cannot find english name: %s", inFilePathServerDesc.c_str());
		//	}
		//} else {
		//	LOGW(TAG, "cannot find names map: %s", inFilePathServerDesc.c_str());
		//}

		bool hasFingerprint = false;
		if (serverDescDoc.size() > 0 && serverDescDoc.isMember(KEY_FINGERPRINT)) {
			Json::Value &serverFingerprintContainer = serverDescDoc[KEY_FINGERPRINT];
			if (serverFingerprintContainer.size() > 0) {

				hasFingerprint = true;
				std::string server_fingerprint = serverFingerprintContainer[INDEX_MAIN_FINGERPRINT]["arg"].asString();
				XstringUtils::trim(server_fingerprint, ' ');
				umap::iterator vHashMapIterator=vFingerprintHashMap.find(server_fingerprint);
				if (vHashMapIterator != vFingerprintHashMap.end()) {
					vHashMapIterator->second.push_back(inFilePathServerDesc);
				} else {
					stringlist vPathList;
					vPathList.push_back(inFilePathServerDesc);
					vFingerprintHashMap.insert(umap::value_type(server_fingerprint, vPathList));
				}

				/*DataJsonIterator &extraInfoIterator = extraInfo.begin();
				for (; !extraInfoIterator.isEnd(); extraInfoIterator.next()) {
					Json::Value &extraInfoDoc = extraInfoIterator.get();
					if (extraInfoDoc.size() > 0 && extraInfoDoc.isMember("extra-info")) {
						std::string extraInfo_extraInfo = extraInfoDoc["extra-info"][INDEX_MAIN_FINGERPRINT]["arg"].asString();
						std::vector<std::string> iArgs;
						XstringUtils::split(extraInfo_extraInfo, ' ', iArgs);
						std::string &extraInfo_fingerprint = iArgs.at(1);

						if (server_fingerprint.find(extraInfo_fingerprint) == 0) {
							std::string inFilePathExtraInfo = extraInfoIterator.getFilePath();
							outFile << inFilePathExtraInfo << std::endl;

							std::ifstream &inFileExtraInfo = extraInfoIterator.getFile();
							inFileExtraInfo.seekg(0);
							XfileUtils::copyFile(inFileExtraInfo, outFile);
							break;
						}
					}
				}*/
			}
		}
		if (!hasFingerprint) {
			LOGW(TAG, "server: %s doesn't have a fingerprint", inFilePathServerDesc.c_str());
		}
	}
	LOGI(TAG, "total count: %d, invalid ip count: %d", totalCount, invalidAddrCount);

	Json::Value::Members vJsonNames;
	std::vector<std::string>::const_iterator vJsonNameIter;
	int vServerCount;

	vJsonNames = vAddrContinentCodeCountMap.getMemberNames();
	vJsonNameIter=vJsonNames.begin();
	vServerCount = 0;
	for (; vJsonNameIter != vJsonNames.end(); vJsonNameIter++) {
		vServerCount+=vAddrContinentCodeCountMap[*vJsonNameIter].asInt();
	}
	
	LOGI(TAG, "continent count: %s, total: %d", vAddrContinentCodeCountMap.toStyledString().c_str(), vServerCount);

	vJsonNames = vAddrCountryCodeCountMap.getMemberNames();
	vJsonNameIter = vJsonNames.begin();
	vServerCount = 0;
	for (; vJsonNameIter != vJsonNames.end(); vJsonNameIter++) {
		vServerCount += vAddrCountryCodeCountMap[*vJsonNameIter].asInt();
	}
	LOGI(TAG, "country count: %s, total: %d", vAddrCountryCodeCountMap.toStyledString().c_str(), vServerCount);

	vJsonNames = vAddrRegisterCountryISOCodeCountMap.getMemberNames();
	vJsonNameIter = vJsonNames.begin();
	vServerCount = 0;
	for (; vJsonNameIter != vJsonNames.end(); vJsonNameIter++) {
		vServerCount += vAddrRegisterCountryISOCodeCountMap[*vJsonNameIter].asInt();
	}
	LOGI(TAG, "registered country count: %s, total: %d", vAddrRegisterCountryISOCodeCountMap.toStyledString().c_str(), vServerCount);

	std::string vServerDigestOutFilePath = vServerOutPath + "ServerDigest.txt";
	boost::filesystem::ofstream vServerDigestOutFile(vServerDigestOutFilePath);
	umap::const_iterator vHashMapIterator = vFingerprintHashMap.cbegin();
	for (; vHashMapIterator != vFingerprintHashMap.cend(); vHashMapIterator++) {
		vServerDigestOutFile << "[" << vHashMapIterator->first << "]" << std::endl;
		stringlist::const_iterator vStrListIterator = vHashMapIterator->second.cbegin();
		for (; vStrListIterator != vHashMapIterator->second.cend(); vStrListIterator++) {
			vServerDigestOutFile << *vStrListIterator << std::endl;
		}
	}
	vServerDigestOutFile.close();

	//size = extraInfo.getFileListSize() / commSize;
	//start = size * commRank;
	//if (commRank = commSize - 1)size = extraInfo.getFileListSize();
	//DataJsonIterator &extraInfoIterator = extraInfo.begin().set(start);

	//index = 0;
	//for (; !extraInfoIterator.isEnd() && index < size; extraInfoIterator.next(), index++) {

	//}

	//MPI_Finalize();
	return 0;
}