#include "XmlMgr.h"
#include <google/protobuf/message.h>
#include "ExcelParseMgr.h"
#include "rapidjson.h"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include <fstream>
#include <iostream>
#include "FilePathMgr.h"
#include <afxwin.h>
#include <numeric>

using namespace google::protobuf;
using namespace rapidxml;

XmlMgr* XmlMgr::m_pInstance = new XmlMgr();

XmlMgr::XmlMgr()
{
}

XmlMgr::~XmlMgr()
{
}

bool XmlMgr::WriteToXml(ISheetT<char>* pSheet,std::string& sTableName, ExcelSheetFormat& esf)
{
	xml_document<> doc;
	xml_node<>* rot = doc.allocate_node(rapidxml::node_pi, doc.allocate_string("xml version='1.0' encoding='utf-8'"));
	doc.append_node(rot);

	xml_node<>* table = doc.allocate_node(node_element, "table", nullptr);
	doc.append_node(table);

	int row(3), col(0);
	while (1)
	{
		xml_node<>* value = doc.allocate_node(node_element, "value", nullptr);
		for (auto& info : esf.sTitle)
		{
			esf.sTitle[col];
			esf.sProtoType[col];

			std::string sData;
			if (!parseData(sData, pSheet, row, col, esf.sProtoType[col]))
			{
				static char error_msg[1024 * 1024] = { 0 };
				sprintf_s(error_msg, "生成配置失败！\n配置表[%s-%s]\n行[%d]\n列[%d]\n[%s]\n解析失败！"
					, sTableName.c_str(), esf.sName.c_str(), row, col, sData.c_str());
				::AfxMessageBox(error_msg, MB_ICONERROR);
				//::AfxMessageBox("生成配置失败！", MB_ICONERROR);
				return false;
			}
			value->append_node(doc.allocate_node(node_element, esf.sTitle[col].c_str(), doc.allocate_string(sData.c_str())));
			++col;

		}
		table->append_node(value);
		++row;
		col = 0;
		int nId = pSheet->readNum(row, col);
		if (nId == 0)
		{
			break;
		}
		
	}

	std::string text;
	rapidxml::print(std::back_inserter(text), doc, 0);
	std::cout << text << std::endl;

	auto& sOutPath = FilePathMgr::GetInstance()->GetOpenOutPath();
	std::fstream fs;
	fs.open(sOutPath + "\\" + sTableName + esf.sName + ".xml", std::fstream::out);
	fs << doc;

	fs.close();

	return true;
}

bool XmlMgr::parseData(std::string& sValue, ISheetT<char>* pSheet, uint32_t nRow, uint32_t nCol, std::string& sType)
{
	if (sType == "int32")
	{
		int64_t nValue = pSheet->readNum(nRow, nCol);
		if (nValue < INT_MIN || nValue > INT_MAX)
		{
			return false;
		}
		auto pValue = pSheet->readStr(nRow, nCol);
		if (pValue != nullptr)
		{
			sValue = pValue;
			return false;
		}
		sValue = std::to_string(nValue);
	}
	else if (sType == "uint32")
	{
		int64_t nValue = pSheet->readNum(nRow, nCol);
		if (nValue < 0 || nValue > UINT_MAX)
		{
			return false;
		}
		auto pValue = pSheet->readStr(nRow, nCol);
		if (pValue != nullptr)
		{
			sValue = pValue;
			return false;
		}
		sValue = std::to_string(nValue);
	}
	else if (sType == "int64")
	{
		int64_t nValue = pSheet->readNum(nRow, nCol);
		sValue = std::to_string(nValue);
		auto pValue = pSheet->readStr(nRow, nCol);
		if (pValue != nullptr)
		{
			sValue = pValue;
			return false;
		}
		sValue = std::to_string(nValue);
	}
	else if (sType == "uint64")
	{
		uint64_t nValue = pSheet->readNum(nRow, nCol);
		sValue = std::to_string(nValue);
		auto pValue = pSheet->readStr(nRow, nCol);
		if (pValue != nullptr)
		{
			sValue = pValue;
			return false;
		}
		sValue = std::to_string(nValue);
	}
	else if (sType == "float")
	{
		double fValue = pSheet->readNum(nRow, nCol);
		if (fValue < FLT_MIN || fValue > FLT_MAX)
		{
			return false;
		}
		sValue = std::to_string(fValue);
		auto pValue = pSheet->readStr(nRow, nCol);
		if (pValue != nullptr)
		{
			sValue = pValue;
			return false;
		}
		sValue = std::to_string(fValue);
	}
	else if (sType == "double")
	{
		double fValue = pSheet->readNum(nRow, nCol);
		sValue = std::to_string(fValue);
		auto pValue = pSheet->readStr(nRow, nCol);
		if (pValue != nullptr)
		{
			sValue = pValue;
			return false;
		}
		sValue = std::to_string(fValue);
	}
	else if (sType == "json")
	{
		rapidjson::Document d;
		d.SetObject();
		auto pValue = pSheet->readStr(nRow, nCol);
		sValue = pValue;
		if (pValue == nullptr)
		{
			return true;
		}
		if (d.Parse(pValue).HasParseError())
		{
			return false;
		}
	}
	else
	{
		auto pValue = pSheet->readStr(nRow, nCol);
		if (pValue == nullptr)
		{
			return true;
		}
		sValue = pValue;
	}
	return true;
}

void XmlMgr::testRead()
{
	rapidxml::file<> fdoc("C:\\Users\\shensijiang\\Desktop\\TTTout\\2Sheet1.xml");
	std::cout << fdoc.data() << std::endl;
	rapidxml::xml_document<> doc;
	doc.parse<0>(fdoc.data());
	std::cout << doc.name() << std::endl;
	//! 获取根节点
	rapidxml::xml_node<>* root = doc.first_node();
	std::cout << root->name() << std::endl;
	//! 获取根节点第一个节点
	rapidxml::xml_node<>* node1 = root->first_node("value");
	std::cout << node1->name() << std::endl;
	rapidxml::xml_node<>* node11 = node1->first_node("param2");
	::AfxMessageBox(node11->value(), MB_ICONERROR);
}