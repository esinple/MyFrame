#include <memory>
#include "MPLogger.h"
#include "SuperNetProxy.h"
#include "MPLogo.h"
#include "MPDump.h"
#include "MPArgs.h"
#include "MPHttpClient.h"
#include "tester.h"
#ifdef WIN_SYSTEM
#else
#include <sys/resource.h>
#endif


//#define gServerName "SuperServer"
const char * gServerName = "SuperServer";
std::string g_sServerName = gServerName;
std::string g_sLogName = "../log/" + g_sServerName + ".log";
std::string g_sCfgName = "../etc/" + g_sServerName + ".cfg";

#ifdef WIN_SYSTEM
BOOL CALLBACK CosonleHandler(DWORD ev)
{
	switch (ev)
	{
	case CTRL_CLOSE_EVENT:
		g_pSuperNetProxy->Final("Close Event");
		g_pSuperNetProxy->CV_Wait();
		break;
	default:
		return false;
	}
	return true;
}
#else
void prog_exit(int sig)
{
	signal(sig, SIG_IGN);
	g_pSuperNetProxy->Final("Linux Signal Close Event!");
//	g_pSuperNetProxy->CV_Wait();
}

void RegisterSignal()
{
	signal(SIGCHLD, SIG_DFL);
	signal(SIGABRT, SIG_DFL);
	signal(SIGSEGV, SIG_DFL);
	signal(SIGHUP, SIG_IGN);
	signal(SIGINT, prog_exit);
	signal(SIGPIPE, prog_exit);
	signal(SIGKILL, prog_exit);
	signal(SIGTERM, prog_exit);
	signal(SIGBUS, prog_exit);
	signal(SIGFPE, prog_exit);
	signal(SIGILL, prog_exit);
	signal(SIGIOT, prog_exit);
	signal(SIGQUIT, prog_exit);
	signal(SIGTRAP, prog_exit);
	signal(SIGSYS, prog_exit);;
	signal(SIGXCPU, prog_exit);
	signal(SIGXFSZ, prog_exit);
}

void RegisterDump()
{
	struct rlimit l;
	l.rlim_cur = RLIM_INFINITY;
	l.rlim_max = RLIM_INFINITY;
	if (setrlimit(RLIMIT_CORE, &l))
	{
		MP_DEBUG("Init Limit Failed");
	}
}
#endif
void super_exit(int seconds)
{
#ifdef WIN_SYSTEM
	for (int i = 0; i < seconds; ++i)
	{
		std::cout << termcolor::red;
		printf("System ShutDown in %d...\n", seconds - i);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
#endif
	exit(0);
}

int main(int argc, char** argv)
{
#ifdef WIN_SYSTEM
	PrintLogo();
	SetConsoleTitleA(gServerName);
	//signal
	SetConsoleCtrlHandler(CosonleHandler, TRUE);
	//dump
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)meplay::ApplicationCrashHandler);
#else
	//signal
	RegisterSignal();
	//dump
	RegisterDump();

#endif
	meplay::MPLogger::GetInstance()->Init(g_sLogName);
    auto uid = MPGUID();

	MP_SYSTEM("%s Start!", gServerName);
	if (!g_pSuperNetProxy->InitServerCfg(g_sCfgName))
	{
		MP_ERROR("%s NetProxy Init Failed!", gServerName);
		super_exit(5);
		return 0;
	}
	MP_SYSTEM("%s NetProxy Init Success!", gServerName);

	MP_SYSTEM("%s NetProxy Start!", gServerName);
	g_pSuperNetProxy->Start();

	MP_SYSTEM("%s NetProxy Final!", gServerName);

	g_pSuperNetProxy->DestroyInstance();
	meplay::MPLogger::DestroyInstance();

	super_exit(5);
    return 0;
}
