#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <google/protobuf/message.h>
#include "DBClient.h"

using INDEX_INFO = std::tuple<std::string, std::vector<std::string>, bool>;
using INDEX_VEC_TYPE = std::vector<INDEX_INFO>;
using INDEX_MAP_TYPE = std::map<uint32_t, INDEX_VEC_TYPE>;

#define KEYS(x,...) \
	std::vector<std::string>{x,##__VA_ARGS__}

#define CREATE_INDEX(COL,INDEX,UNIQUE) \
	std::make_tuple(COL,INDEX,UNIQUE)

class DBClientMgr
{
public:
	static DBClientMgr* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new DBClientMgr();
		}
		return m_pInstance;
	}
	static void DestroyInstance()
	{
		if (m_pInstance != nullptr)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
private:
	DBClientMgr();
	~DBClientMgr();
public:
	//important 需要在调用Connect之前先注册，不然不会设置索引
	void RegisterIndex(uint32_t type, INDEX_INFO index);

	std::shared_ptr<DBClient> GetConn(uint32_t nType);

	bool Connect(uint32_t nType, const std::string& sIp, uint32_t nPort, const std::string& sDBName);

	//int ExecInsert(const std::string& sCollection, bsoncxx::document::value& value);
	int ExecInsert(uint32_t nType, google::protobuf::Message& msg);
	int ExecDelete(uint32_t nType, google::protobuf::Message& msg, bool all = true);
	int ExecUpdate(uint32_t nType, google::protobuf::Message& filter, google::protobuf::Message& update, bool all = true);
	template<typename PB>
	int ExecSelect(uint32_t nType,google::protobuf::Message& filter, std::vector<PB>& results, bool all = true)
	{
		auto pConn = GetConn(nType);
		if (pConn == nullptr)
		{
			return 0;
		}

		return pConn->ExecSelect<PB>(filter, results, all);
	}
private:
	void CreateIndex(std::shared_ptr<DBClient> pConn);

private:
	static DBClientMgr* m_pInstance;

	std::map<uint32_t,std::shared_ptr<DBClient>> m_mClients;
	INDEX_MAP_TYPE m_mIndexs;
};

#define g_pDBMgr DBClientMgr::GetInstance()
