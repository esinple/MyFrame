#include "DBClientMgr.h"
#include "DBClient.h"

DBClientMgr* DBClientMgr::m_pInstance = nullptr;

DBClientMgr::DBClientMgr()
{
}

DBClientMgr::~DBClientMgr()
{
	//
}
void DBClientMgr::CreateIndex(std::shared_ptr<DBClient> pConn)
{
	for (auto& conn : m_mIndexs)
	{
		for (auto& index : conn.second)
		{
			auto& col = std::get<0>(index);
			auto& keys = std::get<1>(index);
			bool unique = std::get<2>(index);
			
			pConn->CreateIndex(col, keys, unique);
		}
		return;
	}
}

void DBClientMgr::RegisterIndex(uint32_t type, INDEX_INFO index)
{
	m_mIndexs[type].emplace_back(index);
}

std::shared_ptr<DBClient> DBClientMgr::GetConn(uint32_t nType)
{
	auto it = m_mClients.find(nType);
	if (it == m_mClients.end())
	{
		return nullptr;
	}
	return it->second;
}

bool DBClientMgr::Connect(uint32_t nType, const std::string& sIp, uint32_t nPort, const std::string& sDBName)
{
	if (GetConn(nType) != nullptr)
	{
		//warning!
		return true;
	}
	auto pDBClient = std::make_shared<DBClient>();
	if (!pDBClient->Connect(sIp, nPort, sDBName))
	{
		return false;
	}

	CreateIndex(pDBClient);
	m_mClients.emplace(nType, pDBClient);
	return true;
}

int DBClientMgr::ExecInsert(uint32_t nType, google::protobuf::Message& msg)
{
	auto pConn = GetConn(nType);
	if (pConn == nullptr)
	{
		return 0;
	}
	return pConn->ExecInsert(msg);
}

int DBClientMgr::ExecDelete(uint32_t nType, google::protobuf::Message& msg, bool all)
{
	auto pConn = GetConn(nType);
	if (pConn == nullptr)
	{
		return 0;
	}
	return pConn->ExecDelete(msg, all);
}

int DBClientMgr::ExecUpdate(uint32_t nType, google::protobuf::Message& filter, google::protobuf::Message& update, bool all)
{
	auto pConn = GetConn(nType);
	if (pConn == nullptr)
	{
		return 0;
	}
	return pConn->ExecUpdate(filter, update, all);
}