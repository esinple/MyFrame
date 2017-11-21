#pragma once
#include <memory>
#include <google/protobuf/message.h>
#include "MPGUID.h"
#include "UserModuleManager.h"

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
private:
	MPGUID m_uid;
	uint64_t m_nUserSock;
	uint64_t m_nGateSock;
	std::string m_sAccount;
	std::string m_sPassword;

	UserModuleManager m_UserModules;
};

typedef std::shared_ptr<GameUser> GameUserPtr;