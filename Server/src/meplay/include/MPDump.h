#pragma once
#ifdef WIN_SYSTEM

#include <time.h>
//#include <windows.h>
#include <DbgHelp.h>
#include <string>

extern const char * gServerName;
#pragma comment(lib,"DbgHelp.lib")

namespace meplay {
	static void CreateDumpFile(const std::string& strDumpFilePathName, EXCEPTION_POINTERS* pException)
	{
		//Dump
		HANDLE hDumpFile = CreateFileA(strDumpFilePathName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
		dumpInfo.ExceptionPointers = pException;
		dumpInfo.ThreadId = GetCurrentThreadId();
		dumpInfo.ClientPointers = TRUE;

		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

		CloseHandle(hDumpFile);
	}

	static long ApplicationCrashHandler(EXCEPTION_POINTERS* pException)
	{
		time_t t = time(0);
		char szDmupName[MAX_PATH];
		tm* ptm = localtime(&t);

		sprintf_s(szDmupName, "%s%04d_%02d_%02d_%02d_%02d_%02d.dmp", gServerName, ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
		meplay::CreateDumpFile(szDmupName, pException);

		FatalAppExitA(-1, szDmupName);

		return EXCEPTION_EXECUTE_HANDLER;
	}
}

#endif