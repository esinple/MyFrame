#pragma once
#include <memory>
#include <google/protobuf/message.h>
#include "MPGUID.h"
#include "UserModuleManager.h"
#include "MPTime.h"

class GameUser
{
public:
	explicit GameUser(const MPGUID uid, uint64_t nUserSock, uint64_t nGateSock);
	virtual ~GameUser();
public:
	inline void SetAccount(const std::string& sAccount) { m_sAccount = sAccount; };
	inline const std::string& GetAccount()const { return m_sAccount; }

	inline void SetPassword(const std::string& sPassword) { m_sPassword = sPassword; };
	inline const std::string& GetPassword()const { return m_sPassword; }

	inline void SetIP(const std::string& ip) { m_sIP = ip; };
	inline const std::string& GetIP()const { return m_sIP; };

	inline void SetLoginTime(const meplay::MPTime& now) { m_LoginTime = now; }
	inline const meplay::MPTime& GetLoginTime()const { return m_LoginTime; }

	inline void SetLogoutTime(const meplay::MPTime& now) { m_LogoutTime = now; }
	inline const meplay::MPTime& GetLogoutTime()const { return m_LogoutTime; }
public:
	void ReuseInit(const MPGUID uid, uint64_t nUserSock, uint64_t nGateSock);
	void Clear();

	MPGUID GetUID()const;
	uint64_t GetUserSock()const;
	uint64_t GetGateSock()const;

	void Send(uint32_t nMsgId, google::protobuf::Message& msg);

	template<typename T>
	T* GetModule(uint32_t nType)
	{
		return m_UserModules.GetModule<T>(nType);
	}

	template<typename T>
	const T* GetModule(uint32_t nType)const
	{
		return m_UserModules.GetModule<T>(nType);
	}

	UserModuleManager& GetAllModules(){ return m_UserModules;}

	void LoadFromDB();
private:
	MPGUID m_uid;
	uint64_t m_nUserSock;
	uint64_t m_nGateSock;
	std::string m_sAccount;
	std::string m_sPassword;
	std::string m_sIP;
	meplay::MPTime m_LoginTime;
	meplay::MPTime m_LogoutTime;

	UserModuleManager m_UserModules;
};

typedef std::shared_ptr<GameUser> GameUserPtr;