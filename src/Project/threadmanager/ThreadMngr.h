
#pragma once

#include <boost/thread/thread_pool.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <map>
#include <vector>

#include "utils/LogUtils.h"

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

class MyThreadPool {
public:
	MyThreadPool();
	~MyThreadPool();

	void init();
	void release();

	template <typename Closure>
	void submit(BOOST_THREAD_FWD_REF(Closure) clouse);

	void join();

private:
	boost::shared_ptr<boost::basic_thread_pool> mBasicThreadPool;
};

class ThreadMngr {
public:
	

	static ThreadMngr &getInstance();

	static ThreadMngr &reset(std::map<std::string, std::string> params);

	void init();

	boost::shared_ptr<MyThreadPool> getThreadPool();

private:
	ThreadMngr();
	~ThreadMngr();

	std::vector<boost::shared_ptr<MyThreadPool>> mPools;

	boost::mutex mMainMutex;

	static ThreadMngr *sInstance;
};
