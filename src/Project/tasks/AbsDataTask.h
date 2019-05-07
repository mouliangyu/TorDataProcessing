#pragma once

#include <string>
#include <fstream>
#include "DataJsonReader.h"
#include "jsoncpp/json.h"

class AbsDataTask :public IJsonParserCallback {
public:
	AbsDataTask();
	~AbsDataTask();
	virtual void init() = 0;
};