#pragma once
#include<string>
#include<direct.h>
#include<stdio.h>
#include<corecrt_io.h>
#include<fstream>
#include<vector>
#include<boost/filesystem.hpp>

class XfileUtils {
public:
	const static int SIZE_4K;

	static int mkdirs(std::string &path);

	static int rmdirs(std::string &path);

	static int copyFile(std::ifstream &in, std::ofstream &out);
};

