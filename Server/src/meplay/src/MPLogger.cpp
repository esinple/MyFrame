#include "MPLogger.h"
#include "termcolor.hpp"
#include <stdio.h>
#include <cassert>

INITIALIZE_EASYLOGGINGPP

using namespace meplay;
MP_SINGLETON_IMPLEMENT(MPLogger);

#ifdef WIN_SYSTEM
#define MP_COL_RED 0x0c
#define MP_COL_YELLOW 0x0e
#define MP_COL_GREEN 0x0a
#define MP_COL_BLUE 0x0b
#define MP_COL_NORMAL 0x07
#else
#include <linux/limits.h>
#define MAX_PATH PATH_MAX
#define sprintf_s snprintf
#define vsnprintf_s vsnprintf
#endif

MPLogger::MPLogger() : m_sFileName("")
{
}

MPLogger::~MPLogger()
{
	el::Helpers::uninstallPreRollOutCallback();
}

int idx = 0;

bool CheckLogFileExist(const char* filename)
{
	/*std::stringstream stream;
	stream << filename << "." << ++idx;
	std::fstream file;
	file.open(stream.str(), std::ios::in);
	if (file)
	{
		file.close();
		return CheckLogFileExist(filename);
	}*/
	std::stringstream ss;
	ss << filename;
	std::fstream file;
	file.open(ss.str(), std::ios::in);
	if (file)
	{
		file.close();
		ss << "." << ++idx;
		return CheckLogFileExist(ss.str().c_str());
	}

	return false;
}

void WriteBackFile(const char* filename,const char* backupname)
{
	std::stringstream stream;
	if (!CheckLogFileExist(backupname))
	{
		stream << backupname;
		if (idx > 0)
		{
			stream << "." << idx;
		}
		rename(filename, stream.str().c_str());
	}
}
void FormatBackUpFileName(const char* filename,char* backupFile, std::string& rollType,struct tm& tmCurrent)
{
	transform(rollType.begin(), rollType.end(), rollType.begin(), ::tolower);
	if (rollType == "month")
	{
		sprintf_s(backupFile, MAX_PATH, "%s.%04d-%02d",
			filename,
			tmCurrent.tm_year + 1900,
			tmCurrent.tm_mon + 1);
	}
	else if (rollType == "day")
	{
		sprintf_s(backupFile, MAX_PATH, "%s.%04d-%02d-%02d",
			filename,
			tmCurrent.tm_year + 1900,
			tmCurrent.tm_mon + 1,
			tmCurrent.tm_mday);
	}
	else if (rollType == "hour")
	{
		sprintf_s(backupFile, MAX_PATH, "%s.%04d-%02d-%02d-%02d",
			filename,
			tmCurrent.tm_year + 1900,
			tmCurrent.tm_mon + 1,
			tmCurrent.tm_mday,
			tmCurrent.tm_hour);
	}
	else if (rollType == "minute")
	{
		sprintf_s(backupFile, MAX_PATH, "%s.%04d-%02d-%02d-%02d-%02d",
			filename,
			tmCurrent.tm_year + 1900,
			tmCurrent.tm_mon + 1,
			tmCurrent.tm_mday,
			tmCurrent.tm_hour,
			tmCurrent.tm_min);
	}

}

void RolloutHandler(const char* filename, std::size_t size, std::tuple<el::base::RollingLogFileBasis, struct tm, std::string> rollingbasis)
{
	char backupFile[MAX_PATH] = { 0 };

	FormatBackUpFileName(filename, backupFile, std::get<2>(rollingbasis), std::get<1>(rollingbasis));


	switch (std::get<0>(rollingbasis))
	{
	case el::base::RollingLogFileBasis::RollLog_FileSize:
	{
		/// 按大小滚动日志文件
		WriteBackFile(filename, backupFile);
	}
	break;
	case el::base::RollingLogFileBasis::RollLog_DateTime:
	{
		/// 按时间滚动日志文件  
		idx = 0;
		WriteBackFile(filename, backupFile);
	}
	break;
	default:
		break;
	}
}

bool MPLogger::Init(const std::string& filename, uint32_t filesize, const std::string& time_roll_type, const std::string& format)
{
	el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
	el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);

	el::Configurations defaultConf;
#ifdef WIN_SYSTEM
	defaultConf.setGlobally(el::ConfigurationType::Filename, filename);
#else
	defaultConf.setGlobally(el::ConfigurationType::Filename, filename);
#endif
	defaultConf.setGlobally(el::ConfigurationType::MaxLogFileSize, std::to_string(filesize));
	defaultConf.setGlobally(el::ConfigurationType::LogFileRollingTime, time_roll_type);
	defaultConf.setGlobally(el::ConfigurationType::Format, format);

	el::Loggers::reconfigureLogger("default", defaultConf);

	el::Helpers::installPreRollOutCallback(RolloutHandler);

	m_sFileName = filename;

	return true;
}

void MPLogger::Info(const char* format, ...)
{
	std::lock_guard<std::mutex> lck(mtx);
	assert(m_sFileName != "");
	char szBuffer[1024 * 10] = { 0 };

	va_list args;
	va_start(args, format);
	vsnprintf_s(szBuffer, sizeof(szBuffer) - 1, format, args);
	va_end(args);

	std::cout << termcolor::white;
	LOG(INFO) << szBuffer;
}

void MPLogger::Warn(const char* format, ...)
{
	std::lock_guard<std::mutex> lck(mtx);
	assert(m_sFileName != "");
	char szBuffer[1024 * 10] = { 0 };

	va_list args;
	va_start(args, format);
	vsnprintf_s(szBuffer, sizeof(szBuffer) - 1, format, args);
	va_end(args);

	std::cout << termcolor::yellow;
	LOG(WARNING) << szBuffer;
}
void MPLogger::Debug(const char* format, ...)
{
#ifdef _DEBUG
	std::lock_guard<std::mutex> lck(mtx);
	assert(m_sFileName != "");
	char szBuffer[1024 * 10] = { 0 };

	va_list args;
	va_start(args, format);
	vsnprintf_s(szBuffer, sizeof(szBuffer) - 1, format, args);
	va_end(args);

	std::cout << termcolor::green;
	LOG(DEBUG) << szBuffer;
#endif
}

void MPLogger::Error(const char* format, ...)
{
	std::lock_guard<std::mutex> lck(mtx);
	assert(m_sFileName != "");
	char szBuffer[1024 * 10] = { 0 };

	va_list args;
	va_start(args, format);
	vsnprintf_s(szBuffer, sizeof(szBuffer) - 1, format, args);
	va_end(args);

	std::cout << termcolor::red;
	LOG(ERROR) << szBuffer;
}

void MPLogger::System(const char* format, ...)
{
	std::lock_guard<std::mutex> lck(mtx);
	assert(m_sFileName != "");
	char szBuffer[1024 * 10] = { 0 };

	va_list args;
	va_start(args, format);
	vsnprintf_s(szBuffer, sizeof(szBuffer) - 1, format, args);
	va_end(args);

	std::cout << termcolor::cyan;
	LOG(SYSTEM) << szBuffer;
}

/*Sleep(1000);

LOG(INFO) << "Log using default file";
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), MP_COL_GREEN);
LOG(DEBUG) << "Log using default file";
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), MP_COL_YELLOW);
LOG(WARNING) << "Log using default file";
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), MP_COL_RED);
LOG(ERROR) << "Log using default file";
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), MP_COL_NORMAL);
LOG(TRACE) << "Log using default file";
LOG(FATAL) << "My first info log using default logger";


void Debug(const char* format, ...);
void Warn(const char* format, ...);
void Error(const char* format, ...);*/

