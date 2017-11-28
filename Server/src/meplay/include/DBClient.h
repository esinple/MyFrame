#pragma once

#include <string>
#include <memory>
#include <tuple>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <google/protobuf/message.h>


using QueryData = std::tuple<std::string, std::string, std::string>;

class DBClient
{
public:
	DBClient();
	~DBClient();
public:
	bool Connect(const std::string& sIp, uint32_t nPort, const std::string& sDBName);

	int ExecInsert(google::protobuf::Message& msg)
	{
		return execInsert(msg);
	}

	int ExecDelete(google::protobuf::Message& msg, bool all = true)
	{
		return execDelete(msg, all);
	}

	int ExecUpdate(google::protobuf::Message& filter, google::protobuf::Message& update, bool all = true)
	{
		return execUpdate(filter,update, all);
	}

	template<typename PB>
	int ExecSelectAll(google::protobuf::Message& filter, std::vector<PB>& results)
	{
		std::vector<std::string> json_results;
		auto count = execQuery(filter, json_results, true);
		for (auto& json_string : json_results)
		{
			PB pb;
			if(!loadPB(json_string,pb))
			{
				continue;
			}
			results.emplace_back(pb);
		}
		return count;
	}

	template<typename PB>
	int ExecSelectOne(google::protobuf::Message& filter, PB& pb)
	{
		std::vector<std::string> json_results;
		auto count = execQuery(filter, json_results, false);
		for (auto& json_string : json_results)
		{
			if (!loadPB(json_string, pb))
			{
				continue;
			}
		}
		return count;
	}

	void CreateIndex(const std::string&, const std::vector<std::string>&, bool);
private:
	int execInsert(google::protobuf::Message& data);

	int execDelete(google::protobuf::Message& filter , bool all = true);

	int execUpdate(google::protobuf::Message& filter, google::protobuf::Message& update, bool all = true);

	int execQuery(google::protobuf::Message& data, std::vector<std::string>& vResult, bool all = true); 

	QueryData pb2json(google::protobuf::Message& pb);
	
	bool loadPB(const std::string& json, google::protobuf::Message&pb);
	
private:
	std::shared_ptr<mongocxx::client> m_pConn;
	mongocxx::database m_DataBase;
	mongocxx::instance ins;
};