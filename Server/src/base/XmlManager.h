#pragma once

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidjson/rapidjson.h"
#include "MPSingleton.h"

using namespace rapidxml;

class XmlManager
{
public:
	MP_SINGLETON_DECLARE(XmlManager);
public:
	static bool ParseJson2Xml(const std::string& sData);
	static bool ParseXml2Json(const std::string& file);
private:
};
