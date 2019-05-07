#pragma once
#include <string>
#include <vector>

class XstringUtils {
public:
	static std::string &trim(std::string &str, char charactor);
	static void split(const std::string &str, char charactor,std::vector<std::string> &container);
	static void split(const std::string &str, const std::string &spliter, std::vector<std::string> &container);
};

