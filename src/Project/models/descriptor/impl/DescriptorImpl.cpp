#include "DescriptorImpl.h"

shared_ptr<std::string> DescriptorImpl::getRawDescriptorBytes() {
	return this->getRawDescriptorBytes(this->offset, this->length);
}

int DescriptorImpl::getRawDescriptorLength() {
	return this->length;
}

shared_ptr<std::vector<std::string>> DescriptorImpl::getAnnotations() {
	return boost::make_shared<std::vector<std::string>>();
}

shared_ptr<std::vector<std::string>> DescriptorImpl::getUnrecognizedLines() {
	return boost::make_shared<std::vector<std::string>>();
}

shared_ptr<boost::filesystem::fstream> DescriptorImpl::getDescriptorFile() {
	return this->descriptorFile;
}

shared_ptr<std::string> DescriptorImpl::getRawDescriptorBytes(int offset, int length) {
	if (offset<this->offset || offset + length>this->offset + this->length || length < 0) {
		return boost::make_shared<std::string>();
	}
	return boost::make_shared<std::string>(rawDescriptorBytes->substr(offset,length));
}
