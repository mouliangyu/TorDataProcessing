#include "ThreadMngr.h"

ThreadMngr *ThreadMngr::sInstance = NULL;

ThreadMngr::ThreadMngr() {
}

ThreadMngr::~ThreadMngr() {
}

ThreadMngr & ThreadMngr::getInstance() {
	if (sInstance == NULL)sInstance = new ThreadMngr();
	return *sInstance;
}

ThreadMngr & ThreadMngr::reset(std::map<std::string, std::string> params) {
	ThreadMngr &tm = getInstance();
	
	//TODO

	return tm;
}

void ThreadMngr::init() {
	boost::unique_lock uniquelock(mMainMutex);
	int i = 0;
	for (; i < mPools.size(); i++) {
		mPools[i]->join();
	}
	while (mPools.size() > 1) {
		mPools.pop_back();
	}
	while (mPools.size()<1) {
		mPools.push_back(boost::make_shared<MyThreadPool>());
	}
}

boost::shared_ptr<MyThreadPool> ThreadMngr::getThreadPool() {
	boost::unique_lock uniquelock(mMainMutex);
	bool haveIdlePool = false;
	int i = 0;
	for (; i < mPools.size(); i++) {
		boost::shared_ptr<MyThreadPool> pool;
		{
			pool=mPools[i];
		}
		
		if (pool.use_count() == 2) {
			return pool;
		}
	}

	boost::shared_ptr<MyThreadPool> newPool = boost::make_shared<MyThreadPool>();
	mPools.push_back(newPool);
	return newPool;

}

MyThreadPool::MyThreadPool() {
	init();
}

MyThreadPool::~MyThreadPool() {
	release();
}

void MyThreadPool::init() {
	mBasicThreadPool = boost::make_shared<boost::basic_thread_pool>();
	
}

void MyThreadPool::release() {
	//TODO
	mBasicThreadPool->join();
	mBasicThreadPool->close();
}

template<typename Closure>
inline void MyThreadPool::submit(BOOST_THREAD_FWD_REF(Closure) clouse) {
	mBasicThreadPool->submit(clouse);
}

void MyThreadPool::join() {
	mBasicThreadPool->join();
}


