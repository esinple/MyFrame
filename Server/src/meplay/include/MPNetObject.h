#pragma once
#include <memory>
#include <string>
#ifdef WIN_SYSTEM
#include <WinSock2.h>
#include <windows.h>
#else
#include <netinet/in.h>
#endif
#include <event2/bufferevent.h>
#include "MPNetDefine.h"

namespace evpp {
	class TCPConn;
	class TCPClient;
}

namespace meplay {
	class MPNet;
	class MPNetObject
	{
	public:
		MPNetObject(const std::shared_ptr<evpp::TCPConn>& pConn);
		virtual ~MPNetObject();
	public:
		MPSOCK GetRealFD()const;
		const std::string& GetIP()const;
		const int GetPort()const;
		std::shared_ptr<evpp::TCPConn>& GetConn();
		
		void Send(const char * msg, const uint32_t len);
	protected:
		MPSOCK m_nFD;
		std::string m_sIP;
		int m_nPort;
		std::shared_ptr<evpp::TCPConn> m_pConn;
	};
	typedef std::shared_ptr<MPNetObject> MPNetObjectPtr;

	class MPTCPClientObj : public MPNetObject
	{
	public:
		//MPTCPClientObj(const std::shared_ptr<evpp::TCPClient>& pClient);
		MPTCPClientObj(evpp::TCPClient* pClient);
		virtual ~MPTCPClientObj();
	public:
		//std::shared_ptr<evpp::TCPClient>& GetClient();
		evpp::TCPClient* GetClient();
		void OnConnectCB();
	private:
		//std::shared_ptr<evpp::TCPClient> m_pClient;
		evpp::TCPClient* m_pClient;
	};
}
