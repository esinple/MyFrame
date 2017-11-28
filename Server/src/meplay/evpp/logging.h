#pragma once

#include "platform_config.h"
#include "MPLogger.h"
#include "termcolor.hpp"

#ifdef __cplusplus
#define GOOGLE_GMPLOG_DLL_DECL           // 使用静态glog库时，必须定义这个
#define GMPLOG_NO_ABBREVIATED_SEVERITIES // 没这个编译会出错,传说因为和Windows.h冲突


#define LOG_FLAG true 
//#define LOG_FLAG false 

#ifdef GOOGLE_STRIP_LOG
#include <glog/logging.h>
#if GOOGLE_STRIP_LOG == 0
#define MPLOG_TRACE LOG(INFO)
#define MPLOG_DEBUG LOG(INFO)
#define MPLOG_INFO  LOG(INFO)
#define MPLOG_TRACE LOG(INFO) << __PRETTY_FUNCTION__ << " this=" << this << " "
#else
#define MPLOG_TRACE if (LOG_FLAG) LOG(INFO)
#define MPLOG_DEBUG if (LOG_FLAG) LOG(INFO)
#define MPLOG_INFO  if (LOG_FLAG) LOG(INFO)
#define MPLOG_TRACE if (LOG_FLAG) LOG(INFO)
#endif

#if GOOGLE_STRIP_LOG <= 1
#define MPLOG_INFO  LOG(WARNING)
#define MPLOG_INFO LOG(WARNING) << __PRETTY_FUNCTION__ << " this=" << this << " "
#else
#define MPLOG_INFO  if (false) LOG(WARNING)
#define MPLOG_INFO if (false) LOG(WARNING)
#endif

#define MPLOG_ERROR LOG(ERROR)
#define MPLOG_FATAL LOG(FATAL)

#else
//#define MPLOG_TRACE std::cout << __FILE__ << ":" << __LINE__ << " "
//#define MPLOG_DEBUG std::cout << __FILE__ << ":" << __LINE__ << " "
//#define MPLOG_INFO  std::cout << __FILE__ << ":" << __LINE__ << " "
//#define MPLOG_ERROR std::cout << __FILE__ << ":" << __LINE__ << " "
//#define MPLOG_FATAL std::cout << __FILE__ << ":" << __LINE__ << " "
#define CHECK_NOTnullptr(val) MPLOG_ERROR << "'" #val "' Must be non nullptr";
#define MPLOG_TRACE if(LOG_FLAG) std::cout <<"\n"
#define MPLOG_DEBUG if(LOG_FLAG) std::cout <<"\n" 
#define MPLOG_INFO  if(LOG_FLAG) std::cout <<"\n"
#define MPLOG_ERROR if(LOG_FLAG) std::cout <<"\n"
#define MPLOG_FATAL if(LOG_FLAG) std::cout <<"\n"

#endif
#endif // end of define __cplusplus

//#ifdef _DEBUG
//#ifdef assert
//#undef assert
//#endif
//#define assert(expr)  { if (!(expr)) { MPLOG_FATAL << #expr ;} }
//#endif
