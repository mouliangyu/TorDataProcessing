
#include "ServerReduceTask.h"

const std::string ServerReduceTask::TAG = "ServerReduceTask";

ServerReduceTask::ServerReduceTask() {
}

ServerReduceTask::~ServerReduceTask() {
}

void ServerReduceTask::onStart() {
	vFingerprintHashMap = umap();
}

void ServerReduceTask::onEnd() {
	LOGI(TAG, "server count: %d", vFingerprintHashMap.size());
}

void ServerReduceTask::onParse(const std::string & path, std::ifstream & file, Json::Value & doc) {
	bool hasFingerprint = false;
	if (doc.size() > 0 && doc.isMember(ServerDescriptorModel::KEY_FINGERPRINT)) {
		Json::Value &serverFingerprintContainer = doc[ServerDescriptorModel::KEY_FINGERPRINT];
		if (serverFingerprintContainer.size() > 0) {

			hasFingerprint = true;
			std::string server_fingerprint = serverFingerprintContainer[(Json::UInt)0]["arg"].asString();
			XstringUtils::trim(server_fingerprint, ' ');
			umap::iterator vHashMapIterator=vFingerprintHashMap.find(server_fingerprint);
			if (vHashMapIterator != vFingerprintHashMap.end()) {
				vHashMapIterator->second.push_back(path);
			} else {
				stringlist vPathList;
				vPathList.push_back(path);
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
		LOGW(TAG, "server: %s doesn't have a fingerprint", path.c_str());
	}
}
