#include "DBClient.h"

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/result/insert_one.hpp>
#include <mongocxx/result/insert_many.hpp>
#include <bsoncxx/json.hpp>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/type_resolver.h>
#include <google/protobuf/util/type_resolver_util.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/writer.h>
#include <rapidjson/rapidjson.h>
//#include <boost/optional.hpp>

#include "MPLogger.h"

enum oper
{
	eOper_Update		= 0x01,
};

DBClient::DBClient()
{
}

DBClient::~DBClient()
{
}

bool DBClient::Connect(const std::string& sIp, uint32_t nPort, const std::string& sDBName)
{
	try {
		mongocxx::uri uri{ "mongodb://" + sIp + ":" + std::to_string(nPort) };
		//mongocxx::client conn{ uri };
		m_pConn = std::make_shared<mongocxx::client>(mongocxx::client{ uri });

		auto admin = m_pConn->database("admin");

		bsoncxx::builder::stream::document ismaster;
		ismaster << "isMaster" << 1;

		auto result = admin.run_command(ismaster.view());

		m_DataBase = m_pConn->database(sDBName);

		auto cursor = m_DataBase.list_collections();
		for (auto& view : cursor)
		{
			//std::string s((char*)view.data(), view.length());
			//std::cout << view["name"].get_utf8().value.to_string() << std::endl;
			bsoncxx::builder::stream::document drop_indexes;
			drop_indexes << "dropIndexes" << view["name"].get_utf8().value << "index" << "*";
			m_DataBase.run_command(drop_indexes.view());
		}

		MP_INFO("Connect DB %s[%s:%d] Success!", sDBName.c_str(), sIp.c_str(), nPort);

		return true;
	}
	catch (...)
	{
		MP_ERROR("Connect DB %s[%s:%d] Failed", sDBName.c_str(), sIp.c_str(), nPort);
		return false;
	}
	return true;
}

bsoncxx::document::view_or_value parseString(google::protobuf::Message& msg, bool update = false)
{
	rapidjson::Document json_document;
	std::string json("");
	google::protobuf::util::MessageToJsonString(msg, &json);

	if (json_document.Parse(json.c_str()).HasParseError())
	{
		return bsoncxx::document::view_or_value{};
	}

	auto bb = bsoncxx::builder::basic::document{};
	using bsoncxx::builder::basic::kvp;

	auto itName = json_document.MemberBegin();
	auto itNameEnd = json_document.MemberEnd();
	for (; itName != itNameEnd; ++itName)
	{
		std::string name = itName->name.GetString();

		auto& value = itName->value;

		/*if (value.IsString())
		{
			auto s = value.GetString();
			bb.append(bsoncxx::builder::basic::kvp(name, s));
			std::cout << name << ":" << s << std::endl;
		}*/
		if (value.IsInt() || value.IsUint())
		{
			bb.append(bsoncxx::builder::basic::kvp(name, (int)value.GetInt()));
		}
		else if (value.IsDouble())
		{
			bb.append(bsoncxx::builder::basic::kvp(name, value.GetDouble()));
		}
		else if (value.IsString())
		{
			rapidjson::Document d;
			rapidjson::Value v(rapidjson::Type::kObjectType);
			auto num = std::atoll(value.GetString());
			if (num > 0)
			{
				v.AddMember("type", 6, d.GetAllocator());
			}
			else
			{
				v.AddMember("type", (int)value.GetType(), d.GetAllocator());
			}
			rapidjson::StringBuffer sb;
			rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
			v.AddMember("value", value, d.GetAllocator());
			v.Accept(writer);
			bb.append(bsoncxx::builder::basic::kvp(name, sb.GetString()));
			//std::cout << sb.GetString() << std::endl;
		}
		else if (value.IsObject() || value.IsArray())
		{
			rapidjson::Document d;
			rapidjson::Value v(rapidjson::Type::kObjectType);
			rapidjson::StringBuffer sb;
			rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
			v.AddMember("type", (int)value.GetType(), d.GetAllocator());
			v.AddMember("value", value, d.GetAllocator());
			v.Accept(writer);
			bb.append(bsoncxx::builder::basic::kvp(name, sb.GetString()));
			//std::cout << sb.GetString() << std::endl;
		}
	}

	if (update)
	{
		auto update_document = bsoncxx::builder::basic::document{};
		update_document.append(bsoncxx::builder::basic::kvp("$set", bb.extract()));
		return update_document.extract();
	}

	return  bb.extract();
}

int DBClient::execInsert(google::protobuf::Message& msg)
{
	try {
		auto name = getPBName(msg);
		auto res = m_DataBase.collection(name).insert_one(parseString(msg));
		return res->result().inserted_count();
	}
	catch (...)
	{
		//error 
		//MP_DEBUG("ExeInsert Error! msg : %s", getPBName(msg).c_str());
		return 0;
	}
	return 0;
}

int DBClient::execDelete(google::protobuf::Message& msg, bool all)
{
	auto name = getPBName(msg);
	auto res = all ?
		m_DataBase.collection(name).delete_many(parseString(msg)) :
		m_DataBase.collection(name).delete_one(parseString(msg));

	return res->result().deleted_count();
}

int DBClient::execUpdate(google::protobuf::Message& filter, google::protobuf::Message& update, bool all)
{
	auto filter_collection = getPBName(filter);
	auto update_collection = getPBName(update);
	if (filter_collection != update_collection)
	{
		return 0;
	}

	try 
	{
		auto res = all ?
			m_DataBase.collection(filter_collection).update_many(parseString(filter), parseString(update, true)) :
			m_DataBase.collection(filter_collection).update_one(parseString(filter), parseString(update, true));

		return res->result().upserted_count();
	}
	catch (...)
	{
		return 0;
	}
}

int DBClient::execQuery(
	google::protobuf::Message& msg,
	std::vector<std::string>& vResult,
	bool all)
{
	auto name = getPBName(msg);
	if (all)
	{
		auto cursor = m_DataBase.collection(name).find(parseString(msg));
		for (auto& view : cursor)
		{
			//std::cout << bsoncxx::to_json(view) << std::endl;
			vResult.emplace_back(bsoncxx::to_json(view));
		}
	}
	else
	{
		auto doc = m_DataBase.collection(name).find_one(parseString(msg));
		if(doc)
		{
			vResult.emplace_back(bsoncxx::to_json(doc.value()));
		}
	}
	return vResult.size();
}

void DBClient::CreateIndex(const std::string& col, const std::vector<std::string>& keys, bool unique)
{
	std::stringstream ss;
	bsoncxx::builder::stream::document index_builder;
	mongocxx::options::index index_options{};
	for (auto& key : keys)
	{
		index_builder << key << 1;
		ss << key << " ";
	}
	index_options.unique(unique);

	m_DataBase[col].create_index(index_builder.view(), index_options);

	MP_INFO("CreateIndex! Collection : %s, Keys : %s ,unique : %d", col.c_str(), ss.str().c_str(), unique);
}

bool DBClient::loadPB(const std::string& json, google::protobuf::Message& msg)
{
	rapidjson::Document d;
	if (d.Parse(json.c_str()).HasParseError())
	{
		return false;
	}

	rapidjson::Document real_doc;
	real_doc.SetObject();

	//for global dd's allocator
	rapidjson::Document dd;

	auto itName = d.MemberBegin();
	auto itNameEnd = d.MemberEnd();
	for (; itName != itNameEnd; ++itName)
	{
		std::string name = itName->name.GetString();
		if (name == "_id")
			continue;
		auto& value = itName->value;
		rapidjson::Value real_name(rapidjson::kStringType);
		real_name.SetString(name.c_str(), name.size(),real_doc.GetAllocator());

		if (value.IsInt())
		{
			real_doc.AddMember(real_name, value.GetInt(), real_doc.GetAllocator());
			//std::cout << real_name.GetString() << value.GetInt() << std::endl;
		}
		else if (value.IsUint())
		{
			real_doc.AddMember(real_name, value.GetUint(), real_doc.GetAllocator());

			//std::cout << real_name.GetString() << value.GetUint() << std::endl;
		}
		else if (value.IsDouble())
		{
			real_doc.AddMember(real_name, value.GetDouble(), real_doc.GetAllocator());

			//std::cout << real_name.GetString() << value.GetDouble() << std::endl;
		}
		else if (value.IsString())
		{
			if (dd.Parse(value.GetString()).HasParseError())
			{
				//maybe error! maybe its string!
				continue;
				//return false;
			}
			
			switch (dd["type"].GetInt())
			{
			case rapidjson::kNumberType:
			{
				int64_t ll = std::atoll(dd["value"].GetString());
				real_doc.AddMember(real_name, ll, real_doc.GetAllocator());
				//real_doc.AddMember(real_name, dd["value"], real_doc.GetAllocator());
			}
			break;
			case rapidjson::kObjectType:
			{
				real_doc.AddMember(real_name, dd["value"], real_doc.GetAllocator());
			}
			break;
			case rapidjson::kArrayType:
			{
				real_doc.AddMember(real_name, dd["value"], real_doc.GetAllocator());
			}
			break;
			case rapidjson::kStringType:
			{
				//rapidjson::Value v(rapidjson::kStringType);
				//v.SetString(dd["value"].GetString(),dd["value"].GetStringLength(),real_doc.GetAllocator());
				real_doc.AddMember(real_name, dd["value"], real_doc.GetAllocator());
			}
			default:
				break;
			}
		}

		//real_doc.AddMember("obj", real,real_doc.GetAllocator());
	}

	rapidjson::StringBuffer sb;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
	//d.Accept(writer);
	//std::cout << sb.GetString() << std::endl;
	real_doc.Accept(writer);
	//std::cout << sb.GetString() << std::endl;
	google::protobuf::util::JsonStringToMessage(sb.GetString(), &msg);
	return true;
}

QueryData DBClient::pb2json(google::protobuf::Message& pb)
{
	std::string sJson;
	google::protobuf::util::MessageToJsonString(pb, &sJson);

	return std::move(std::make_tuple(pb.GetDescriptor()->name(), sJson, pb.SerializeAsString()));
}

std::string DBClient::getPBName(google::protobuf::Message& pb)
{
	auto sFullName = pb.GetTypeName();
	auto nPos = sFullName.find_last_of('.');
	if (nPos == std::string::npos)
	{
		return sFullName;
	}
	return sFullName.substr(nPos + 1, sFullName.size());
}