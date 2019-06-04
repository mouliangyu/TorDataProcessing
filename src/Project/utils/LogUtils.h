#pragma once
#include <iostream>

#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sinks.hpp>

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/log/support/date_time.hpp>

#include <boost/log/trivial.hpp>

#include <boost/format.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

enum severity_level {
	info,
	debug,
	warning,
	error,
	fatal
};



class Logger {

public:

	typedef sinks::synchronous_sink<sinks::text_ostream_backend> sync_sink;
	typedef sinks::asynchronous_sink<sinks::text_ostream_backend> asyn_sink;
	typedef sinks::asynchronous_sink<sinks::text_file_backend> asyn_file_sink;

	typedef src::severity_logger_mt<severity_level> logger_mt;

	static Logger &getInstance();

	~Logger();

	int init();
	int setLogLevel(severity_level level);
	int log(severity_level level, const std::string &tag, const std::string &msg);
	int log(severity_level level, const std::string &tag, const char *fmt, ...);

	boost::shared_ptr<logger_mt> getInnerLogger();

private:
	Logger();

	static Logger *sInstance;

	severity_level mLogLevel;

	bool mInitialized;
	
	std::string mLogFileFullName;
	boost::shared_ptr<sync_sink> mConsoleSink;
	boost::shared_ptr<asyn_file_sink> mFileSink;
	boost::shared_ptr<logger_mt> mInnerLogger;
	attrs::mutable_constant<std::string> mTAG;
};

//#define LOGI(tag,msg) Logger::getInstance().log(info,tag,msg)
//#define LOGD(tag,msg) Logger::getInstance().log(debug,tag,msg)
//#define LOGW(tag,msg) Logger::getInstance().log(warning,tag,msg)
//#define LOGE(tag,msg) Logger::getInstance().log(error,tag,msg)
//#define LOGF(tag,msg) Logger::getInstance().log(fatal,tag,msg)

//#define LOGI(tag,fmt,...) Logger::getInstance().log(info,tag,fmt,__VA_ARGS__)
//#define LOGD(tag,fmt,...) Logger::getInstance().log(debug,tag,fmt,__VA_ARGS__)
//#define LOGW(tag,fmt,...) Logger::getInstance().log(warning,tag,fmt,__VA_ARGS__)
//#define LOGE(tag,fmt,...) Logger::getInstance().log(error,tag,fmt,__VA_ARGS__)
//#define LOGF(tag,fmt,...) Logger::getInstance().log(fatal,tag,fmt,__VA_ARGS__)

#define LOGI(tag,fmt,...)\
	Logger::getInstance().log(info,tag,"%s(): "##fmt,__FUNCTION__,__VA_ARGS__)
#define LOGD(tag,fmt,...)\
	Logger::getInstance().log(debug,tag,"%s(): "##fmt,__FUNCTION__,__VA_ARGS__)
#define LOGW(tag,fmt,...)\
	Logger::getInstance().log(warning,tag,"%s(): "##fmt,__FUNCTION__,__VA_ARGS__)
#define LOGE(tag,fmt,...)\
	Logger::getInstance().log(error,tag,"%s(): "##fmt,__FUNCTION__,__VA_ARGS__)
#define LOGF(tag,fmt,...)\
	Logger::getInstance().log(fatal,tag,"%s(): "##fmt,__FUNCTION__,__VA_ARGS__)

#define SET_LOG_LEVEL(level) Logger::getInstance().setLogLevel(level)