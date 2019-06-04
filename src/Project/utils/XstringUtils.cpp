#include "XstringUtils.h"

std::string & XstringUtils::trim(std::string & str, char charactor) {
	std::string::const_iterator iIterator = str.cbegin();
	while (iIterator != str.cend()) {
		if (*iIterator == charactor) {
			str.erase(iIterator);
		} else {
			iIterator++;
		}
	}
	return str;
}

void XstringUtils::split(const std::string & str, char charactor, std::vector<std::string>& container) {
	std::string::const_iterator iIterator = str.cbegin();
	std::string temp;
	while (iIterator!=str.cend()) {
		if (*iIterator == charactor) {
			if (temp.size() > 0) {
				container.push_back(temp);
				temp.clear();
			}
		} else {
			temp.push_back(*iIterator);
		}
		++iIterator;
	}
	if (temp.size() > 0) {
		container.push_back(temp);
	}
}

void XstringUtils::split(const std::string & str, const std::string & spliter, std::vector<std::string>& container) {
	std::string right = str;
	std::string left;
	size_t spliterLen = spliter.size();
	size_t index = 0;
	while ((index=right.find(spliter))!=std::string::npos) {
		if (index > 0) {
			left = right.substr(0, index);
			container.push_back(left);
		}
		right.erase(0, index + spliterLen);
	}
	container.push_back(right);
}
