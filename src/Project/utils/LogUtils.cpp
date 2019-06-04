#include "LogUtils.h"

// The formatting logic for the severity level
template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (
	std::basic_ostream< CharT, TraitsT >& strm, severity_level lvl) {
	static const char* const str[] =
	{
		"I",
		"D",
		"W",
		"E",
		"F"
	};
	if (static_cast<std::size_t>(lvl) < (sizeof(str) / sizeof(*str)))
		strm << str[lvl];
	else
		strm << static_cast<int>(lvl);
	return strm;
}
template< typename CharT, typename TraitsT >
inline std::basic_ostream<CharT, TraitsT>& operator<<(
	std::basic_ostream< CharT, TraitsT >& strm, boost::log::aux::process::id & processID) {
	strm << processID.native_id;
	return strm;
}
template< typename CharT, typename TraitsT >
inline std::basic_ostream<CharT, TraitsT>& operator<<(
	std::basic_ostream< CharT, TraitsT >& strm, boost::log::aux::thread::id & threadID) {
	strm << threadID.native_id;
	return strm;
}

Logger *Logger::sInstance = nullptr;

Logger::Logger():
	mLogLevel(info),
	mLogFileFullName("D:\\WorkSpace\\GraduationDesign\\TorDataProcessing\\Project\\log\\default.log"),
	//mLogFileFullName("default.log"),
	mTAG(""),
	mInitialized(false)
{
	init();
}


Logger::~Logger() {
	boost::shared_ptr<logging::core> logCore = logging::core::get();

	logCore->remove_sink(mFileSink);
	mFileSink->stop();
	mFileSink->flush();
	mFileSink.reset();

	delete sInstance;
	sInstance = nullptr;
}

int Logger::init() {
	if (mInitialized)
		return 0;
	else
		mInitialized = true;
	try {
		boost::shared_ptr<logging::core> logCore = logging::core::get();
		
		auto vFormat=expr::format("%1%:%2% [%3%] [%4%] <%5%> [%6%]: %7%")
			% expr::attr<boost::log::aux::process::id>("ProcessID")
			% expr::attr<boost::log::aux::thread::id>("ThreadID")
			% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d, %H:%M:%S")
			% expr::format_date_time< attrs::timer::value_type >("Uptime", "%O:%M:%S")
			//% expr::format_named_scope("Scope", keywords::format = "%n (%f:%l)")
			% expr::attr< severity_level >("Severity")
			% expr::attr< std::string >("TAG")
			% expr::message;

		mConsoleSink = logging::add_console_log(std::clog, keywords::format = vFormat);

		boost::shared_ptr<sinks::text_file_backend> vFileBackend = boost::make_shared<sinks::text_file_backend>(keywords::file_name = mLogFileFullName.c_str());
		mFileSink = boost::make_shared<asyn_file_sink>(vFileBackend);
		mFileSink->set_formatter(vFormat);

		logCore->add_sink(mFileSink);

		logging::add_common_attributes();
		//logging::core::get()->add_thread_attribute("Scope", attrs::named_scope());
		logging::core::get()->add_global_attribute("ProcessID", attrs::current_process_id());
		logging::core::get()->add_global_attribute("ThreadID", attrs::current_thread_id());

		mInnerLogger = boost::make_shared<logger_mt>();

		mInnerLogger->add_attribute("Uptime", attrs::timer());
		mInnerLogger->add_attribute("TAG", mTAG);
		//setLogLevel(mLogLevel);
	} catch (std::exception & e) {
		std::cout << "Base Fatal: " << e.what() << std::endl
			<< "\tat " << __FUNCTION__ << std::endl;
		return -1;
	}
	
	return 0;
}

Logger & Logger::getInstance() {
	if (sInstance == nullptr)sInstance = new Logger;
	return *sInstance;
}

int Logger::setLogLevel(severity_level level) {
	mConsoleSink->set_filter(expr::attr<severity_level>("Severity") >= level);
	mFileSink->set_filter(expr::attr<severity_level>("Severity") >= level);
	return 0;
}

int Logger::log(severity_level level, const std::string & tag, const std::string & msg) {
	try {
		mTAG.set(tag);
		BOOST_LOG_SEV(*mInnerLogger, level) << msg;
		mTAG.set("");
	} catch (std::exception & e) {
		std::cout << "Base Fatal: " << e.what() << std::endl
			<< "\tat " << __FUNCTION__ << std::endl;
		return -1;
	}
	return 0;
}

int Logger::log(severity_level level, const std::string & tag, const char * fmt, ...) {
	mTAG.set(tag);

	va_list arg_ptr;
	va_start(arg_ptr, fmt);

	constexpr int vInterval = 256;
	int vLogSize = vInterval;
	char *vLogBuf = (char *)malloc(vLogSize * sizeof(char));
	int vReceiveNum=vsnprintf(vLogBuf, vLogSize, fmt, arg_ptr);

	while (vReceiveNum>=vLogSize) {
		vLogSize += vInterval;
		char *vLogBufTmp = (char *)malloc(vLogSize * sizeof(char));
		free(vLogBuf);
		vLogBuf = vLogBufTmp;
		vReceiveNum = vsnprintf(vLogBuf, vLogSize, fmt, arg_ptr);
	}

	BOOST_LOG_SEV(*mInnerLogger, level) << vLogBuf;

	free(vLogBuf);

	va_end(arg_ptr);

	mTAG.set("");

	return 0;
}

boost::shared_ptr<Logger::logger_mt> Logger::getInnerLogger() {
	return mInnerLogger;
}
