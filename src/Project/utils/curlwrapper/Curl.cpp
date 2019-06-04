
#include "Curl.h"

Curl *Curl::sInstance = nullptr;

Curl & Curl::getInstance() {
	if (sInstance == nullptr)sInstance = new Curl();
	return *sInstance;
}

Curl::Curl() {
	init();
}

Curl::~Curl() {
	release();
}

void Curl::init() {
	curl_global_init(CURL_GLOBAL_ALL);
}

void Curl::release() {
	curl_global_cleanup();
}
