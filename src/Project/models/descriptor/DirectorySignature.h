
#pragma once

#include <string>

class DirectorySignature {
	virtual shared_ptr<std::string> getAlogrithm() = 0;
	virtual shared_ptr<std::string> getIdentiry() = 0;
	virtual shared_ptr<std::string> getSigningKeyDigestSha1Hex() = 0;
	virtual shared_ptr<std::string> getSignature() = 0;
};