
#pragma once
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

class Descriptor {
public:
	virtual shared_ptr<std::string> getRawDescriptorBytes() = 0;
	virtual int getRawDescriptorLength() = 0;
	virtual shared_ptr<std::vector<std::string>> getAnnotations() = 0;
	virtual shared_ptr<std::vector<std::string>> getUnrecognizedLines() = 0;
	virtual shared_ptr<boost::filesystem::fstream> getDescriptorFile() = 0;
};