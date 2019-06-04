
#pragma once

#include <boost/make_shared.hpp>
#include "../Descriptor.h"

class DescriptorImpl :public Descriptor {
public:
	virtual shared_ptr<std::string> getRawDescriptorBytes() override;
	virtual int getRawDescriptorLength() override;
	virtual shared_ptr<std::vector<std::string>> getAnnotations() override;
	virtual shared_ptr<std::vector<std::string>> getUnrecognizedLines() override;
	virtual shared_ptr<boost::filesystem::fstream> getDescriptorFile() override;
public:
	static const std::string NL;
	static const std::string SP;
protected:
	shared_ptr<std::string> rawDescriptorBytes;
	int offset;
	int length;
private:
	shared_ptr<boost::filesystem::fstream> descriptorFile;

protected:
	shared_ptr<std::string> getRawDescriptorBytes(int offset, int length);
	int findFirstIndexOfKey();
};