#pragma once

#include <curl/curl.h>

class Curl {
public:
	
	static Curl &getInstance();

private:
	Curl();
	~Curl();

	void init();
	void release();

	static Curl *sInstance;
};

