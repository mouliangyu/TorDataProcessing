
#pragma once

#include <string>
#include <set>
#include <map>
#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

class NetworkStatusEntry {
	virtual shared_ptr<std::string> getStatusEntryBytes() = 0;
	virtual shared_ptr<std::string> getNickname() = 0;
	virtual shared_ptr<std::string> getFingerprint() = 0;
	virtual shared_ptr<std::string> getDescriptor() = 0;
	virtual long getPublishedMillis() = 0;
	virtual shared_ptr<std::string> getAddress() = 0;
	virtual int getOrPort() = 0;
	virtual int getDirPort() = 0;
	virtual shared_ptr<std::set<std::string>> getMicrodescriptorDigestsSha256Base64() = 0;
	virtual shared_ptr<std::vector<std::string>> getOrAddresses() = 0;
	virtual shared_ptr<std::set<std::string>> getFlags() = 0;
	virtual shared_ptr<std::string> getVersion() = 0;
	virtual shared_ptr<std::map<std::string,std::set<long>>> getProtocols() = 0;
	virtual long getBandwidth() = 0;
	virtual long getMeasured() = 0;
	virtual bool getUnmeasured() = 0;
	virtual shared_ptr<std::string> getDefaultPolicy() = 0;
	virtual shared_ptr<std::string> getPortList() = 0;
	virtual shared_ptr<std::string> getMasterKeyEd25519() = 0;
};