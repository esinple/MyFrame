#ifndef _MP_LOGGER_H_
#define _MP_LOGGER_H_

#include "easylogging++.h"
#include "MPSingleton.h"
#include <mutex>

#define MP_INFO		meplay::MPLogger::GetInstance()->Info
#define MP_DEBUG	meplay::MPLogger::GetInstance()->Debug
#define MP_WARN		meplay::MPLogger::GetInstance()->Warn
#define MP_ERROR	meplay::MPLogger::GetInstance()->Error
#define MP_SYSTEM	meplay::MPLogger::GetInstance()->System

namespace meplay {
	class MPLogger final
	{
		MP_SINGLETON_DECLARE(MPLogger);

	public:
		bool Init(
			const std::string& filename, 
			uint32_t filesize = 10485760,//1024 * 1024 * 10 = 10mb
			const std::string& time_roll_type = "hour",
			const std::string& format = "[%datetime{%Y-%M-%d][%H:%m:%s}][%level]%msg"
		);

		void Info(const char* format, ...);
		void Warn(const char* format, ...);
		void Debug(const char* format, ...);
		void Error(const char* format, ...);
		void System(const char* format, ...);

	private:
		std::string m_sFileName;
		std::mutex mtx;
	};
};

#endif
