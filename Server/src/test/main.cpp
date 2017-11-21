#include <memory>

#include "MPLogger.h"
#include "SuperNetProxy.h"
//#ifdef WIN_SYSTEM
//#endif

#define SERVERNAME "SuperServer"
std::string g_sServerName = SERVERNAME;
std::string g_sLogName = "../log/" + g_sServerName + ".log";
std::string g_sCfgName = "../etc/" + g_sServerName + ".cfg";

#ifdef WIN_SYSTEM
BOOL CALLBACK CosonleHandler(DWORD ev)
{
	switch (ev)
	{
	case CTRL_CLOSE_EVENT:
		g_pNetProxy->Final("Close Event");
		break;
	default:
		return false;
	}
	return true;
}
#else
#endif

int main(int argc, char** argv)
{
	meplay::MPLogger::GetInstance()->Init(g_sLogName);

#ifdef WIN_SYSTEM
	SetConsoleCtrlHandler(CosonleHandler, TRUE);
#else
#endif
	if (!g_pNetProxy->InitServerCfg(g_sCfgName))
	{
		MP_ERROR("%s NetProxy Init Failed!", SERVERNAME);
		return 0;
	}
	MP_SYSTEM("%s NetProxy Init Success!", SERVERNAME);

	MP_SYSTEM("%s NetProxy Start!", SERVERNAME);
	g_pNetProxy->Start();

	MP_SYSTEM("%s NetProxy Final!", SERVERNAME);

	g_pNetProxy->DestroyInstance();
	meplay::MPLogger::DestroyInstance();

    return 0;
}