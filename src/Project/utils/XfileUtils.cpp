#include "XfileUtils.h"

const int XfileUtils::SIZE_4K = 1024 * 4;

int XfileUtils::mkdirs(std::string & path) {
	int pathLen = path.length();
	char *cPath = (char *)malloc(sizeof(char)*(pathLen + 1));
	strncpy(cPath, path.c_str(), pathLen);
	cPath[pathLen] = '\0';

	std::string temp;
	char *cPos = cPath;
	for (; *cPos != '\0'; cPos++) {
		if (*cPos == '\\' || *cPos == '/') {
			if (_access(temp.c_str(), 0) != 0) {
				if (_mkdir(temp.c_str()) != 0) {
					return -1;
				}
			}
		}
		temp.push_back(*cPos);
	}
	return 0;
}

int XfileUtils::rmdirs(std::string & path) {
	int vRet = 0;

	std::string::const_iterator vIterator = path.cbegin();
	std::vector<std::string> vStack;
	std::string temp;
	while (vIterator != path.cend()) {
		if (*vIterator == '\\' || *vIterator == '/') {
			if (temp.size() > 0) {
				if (temp == "..") {
					vStack.pop_back();
				} else if (temp != ".") {
					vStack.push_back(temp);
				}
				temp.clear();
			}
		} else {
			temp.push_back(*vIterator);
		}
		vIterator++;
	}

	std::string formatted;
	std::vector<std::string>::const_iterator vStackIterator = vStack.cbegin();
	if (vStackIterator != vStack.cend()) {
		if (path.at(0) == '\\' || path.at(0) == '/') {

		}
	}

	for (; vStackIterator != vStack.cend(); vStackIterator++) {
		
	}

	std::string root = formatted;
	while (root.at(formatted.length()-1)!='\\'&&formatted.at(formatted.length()-1)!='/') {
		root.erase(formatted.length() - 1, 1);
	}

	struct _finddata_t fileInfo = { 0 };
	intptr_t fileHandler = _findfirst(formatted.c_str(), &fileInfo);
	if (fileHandler != -1) {
		do {
			std::string cur = root + fileInfo.name;
			if (fileInfo.attrib&_A_SUBDIR) {
				if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0) {
					std::string subdirs = cur + "\\*";
					vRet = rmdirs(subdirs);
				}
				if (vRet == 0)vRet = remove(cur.c_str());
			}
		} while (_findnext(fileHandler, &fileInfo) == 0);
		_findclose(fileHandler);
	} else {
		vRet = -1;
	}

	return 0;
}

int XfileUtils::copyFile(std::ifstream & in, std::ofstream & out) {
	if (!in.is_open() || !out.is_open()) {
		return -1;
	} else {
		char buffer[SIZE_4K];
		int totalBytes = 0;
		while (in) {
			in.read(buffer, SIZE_4K);
			out.write(buffer, in.gcount());
			totalBytes += in.gcount();
		}
		return totalBytes;
	}
	return 0;
}
