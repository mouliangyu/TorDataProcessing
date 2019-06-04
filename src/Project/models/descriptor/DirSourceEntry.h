
#pragma once

#include <string>
#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

class DirSourceEntry {
	virtual shared_ptr<std::string> getDirSourceEntryBytes() = 0;
	virtual shared_ptr<std::string> getNickname() = 0;
	virtual shared_ptr<std::string> getIdentity() = 0;
	virtual shared_ptr<std::string> getHostName() = 0;
	virtual shared_ptr<std::string> getIP() = 0;
	virtual int getDirPort() = 0;
	virtual int getOrPort() = 0;
	virtual bool isLegacy() = 0;
	virtual shared_ptr<std::string> getContactLine() = 0;
	virtual shared_ptr<std::string> getVoteDigestSha1Hex() = 0;
};