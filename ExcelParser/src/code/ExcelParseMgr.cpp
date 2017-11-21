#include "ExcelParseMgr.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include "XmlMgr.h"
#include "FilePathMgr.h"
#include <afxwin.h>
#include <afxcmn.h>
#include "Resource.h"
#include <chrono>
#include <thread>
#include "XmlMgr.h"

ExcelParseMgr* ExcelParseMgr::m_pInstance = nullptr;

ExcelParseMgr::ExcelParseMgr()
	: m_bFinish(false)
{
}

ExcelParseMgr::~ExcelParseMgr()
{
}

bool ExcelParseMgr::IsFinish()const
{
	return m_bFinish;
}

void ExcelParseMgr::ParseExcel()
{
	if (!initExcelPath())
	{
		return;
	}
	if (!parseExcelFormat())
	{
		return;
	}

	//放弃了暂时
	if (!saveToProto())
	{
	}

	addProgress(10000);
	updateProgressInfo("生成完成！");
	m_bFinish = true;

	::AfxMessageBox("生成配置完成！");
}

bool ExcelParseMgr::initExcelPath()
{
	auto& sOpenPath = FilePathMgr::GetInstance()->GetOpenInPath();
	WIN32_FIND_DATAA FindFileData;
	auto& sTargetFile = sOpenPath + "\\*.xlsx";

	HANDLE hFind = FindFirstFileA(sTargetFile.c_str(), &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return false;
	}

	do
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
		}
		else if (FindFileData.cFileName[0] != '~')
		{
			ExcelFormat ef;
			ef.sFilePath = sOpenPath;
			ef.sFileName = FindFileData.cFileName;
			ef.sFileName = ef.sFileName.substr(0, ef.sFileName.size() - 5);
			bool bLegal(true);
			for (auto& s : ef.sFileName)
			{
				if ((s < 'a' || s > 'z')
					&& (s < 'A' || s > 'Z') 
					&& (s < '1' || s > '9') 
					&& s != '_') 
				{
					bLegal = false;
					break;
				}
			}
			if (bLegal)
			{
				m_vExcelFormats.emplace_back(ef);
			}
		}
	} while (FindNextFileA(hFind, &FindFileData));

	FindClose(hFind);


	return true;
}

bool ExcelParseMgr::parseExcelFormat()
{
	int nProgress = 10000 / m_vExcelFormats.size();
	for (auto& ef : m_vExcelFormats)
	{
		auto pBook = xlCreateXMLBookA();
		if (pBook == nullptr)
		{
			return false;
		}

		auto sFullPath = ef.sFilePath + "\\" + ef.sFileName + ".xlsx"; 
		if (!pBook->load(sFullPath.c_str()))
		{
			pBook->release();
			return false;
		}

		auto nCount = pBook->sheetCount();
		for (int i = 0; i < nCount; ++i)
		{
			auto pSheet = pBook->getSheet(i);
			if (pSheet == nullptr)
			{
				continue;
			}

			if (!parseSheetHead(pSheet, ef))
			{
				continue;
			}

			if (!XmlMgr::GetInstance()->WriteToXml(pSheet, ef.sFileName,ef.vSheetFormat.back()))
			{
				//::AfxMessageBox(_T("生成配置失败！"), MB_ICONERROR);
				return false;
			}
		}

		pBook->release();

		addProgress(nProgress);
	}
	return true;
}

bool ExcelParseMgr::parseSheet(ISheetT<char>* pSheet,ExcelFormat& ef)
{
	
	return true;
}

bool ExcelParseMgr::parseSheetHead(ISheetT<char>* pSheet,ExcelFormat& ef)
{
	updateProgressInfo(ef.sFileName + " " + pSheet->name());
	ExcelSheetFormat esf;
	esf.sName = pSheet->name();

	auto s = pSheet->readStr(0, 0);
	if (s == nullptr)
	{
		return false;
	}
	if (_stricmp(s, "id") != 0)
	{
		return false;
	}

	int i(0);
	while (1)
	{
		auto sName = pSheet->readStr(0, i);
		if (sName == nullptr)
		{
			break;
		}

		esf.sTitle.emplace_back(sName);

		auto sType = pSheet->readStr(2, i);
		if (sType == nullptr)
		{
			//有名字，没type
			return false;
		}
		esf.sType.emplace_back(sType);

		esf.sProtoType.emplace_back(parseToProtoType(esf.sType.back()));
		++i;
	}
	ef.vSheetFormat.emplace_back(esf);

	return true;
}

bool ExcelParseMgr::saveToProto()
{
	std::string sData;
	sData.append("syntax = \"proto3\";\n\n");

	for (auto& ef : m_vExcelFormats)
	{
		if (!saveToProto(ef, sData))
		{
			return false;
		}
	}

	std::fstream fs;
	fs.open("GameResource.proto", std::fstream::out);
	fs << sData;

	fs.close();
	return true;
}

bool ExcelParseMgr::saveToProto(ExcelFormat& ef, std::string& sData)
{
	for (auto& esf : ef.vSheetFormat)
	{
		sData.append("message " + ef.sFileName + "_" + esf.sName + "\n{\n");

		std::vector<std::string> sSubProtos;
		for (int i = 0, max = esf.sTitle.size(); i < max; ++i)
		{
			auto sProtoType = esf.sProtoType[i];
			if (sProtoType == "object")
			{
				std::string sSubProto;
				std::string sSubProtoName = esf.sName + "_" + esf.sTitle[i];
				rapidjson::Document d;
				d.SetObject();
				if (d.Parse(esf.sType[i].c_str()).HasParseError())
				{
					return false;
				}
				if (!saveToSubProto(sSubProtoName, d, sSubProto, "object"))
				{
					return false;
				}
				sSubProtos.emplace_back(sSubProto);
				sData.append("\t" + sSubProtoName + " " + esf.sTitle[i] + " = " + std::to_string(i + 1) + ";");
				sData.append("\n");
			}
			else if (sProtoType == "json")
			{
				sData.append("\tstring " + esf.sTitle[i] + " = " + std::to_string(i + 1) + ";");
				sData.append("\n");
			}
			else
			{
				sData.append("\t" + sProtoType + " " + esf.sTitle[i] + " = " + std::to_string(i + 1) + ";");
				sData.append("\n");
			}

		}
		sData.append("}\n\n");

		for (auto& sSubProto : sSubProtos)
		{
			sData.append(sSubProto + "\n");
		}
	}

	return true;
}

bool ExcelParseMgr::saveToSubProto(const std::string& sParentName,Value& v, std::string& sData,std::string sParentType)
{
	sData.append("message " + sParentName + "\n{\n");
	std::string sSubData;

	int i(0);
	auto it = v.Begin();
	auto itEnd = v.End();
	for (; it != itEnd; ++it)
	{
		if (!saveSubProtoIndex(*it, sParentName, sData, sSubData, i++))
		{
			return false;
		}
	}
	sData.append("}\n\n");

	sData.append(sSubData);
	return true;
}

bool ExcelParseMgr::saveSubProtoIndex(Value& v,const std::string& sParentName,std::string& sData, std::string& sSubData,int index)
{
	rapidjson::Value name, type, value;
	if (!getNameTypeValue(v, name, type, value))
	{
		return false;
	}

	std::string sName = name.GetString();
	if (sName.empty())
	{
		return false;
	}
	std::string sType = type.GetString();
	if (sType.empty())
	{
		return false;
	}

	auto sProtoType = parseToProtoType(sType);
	if (sProtoType == "object")
	{
		auto sSubProtoName = sParentName + "_" + sName;
		sData.append("\t" + sSubProtoName + " " + sName + " = " + std::to_string(index + 1) + ";\n");
		saveToSubProto(sSubProtoName, value, sSubData, sProtoType);
	}
	else if (sProtoType == "object_array")
	{
		auto sSubProtoName = sParentName + "_" + sName;
		sData.append("\trepeated " + sSubProtoName + " " + sName + " = " + std::to_string(index + 1) + ";\n");
		saveToSubProto(sSubProtoName, value, sSubData, sProtoType);
	}
	else
	{
		sData.append("\t" + sProtoType + " " + sName + " = " + std::to_string(index + 1) + ";\n");
	}
	return true;
}

bool ExcelParseMgr::getNameTypeValue(Value& d, Value& name, Value& type, Value& value)
{
	if (!d.HasMember("name"))
	{
		return false;
	}
	name = d["name"];

	if (!d.HasMember("type"))
	{
		return false;
	}
	type = d["type"];
	
	if (!d.HasMember("value"))
	{
		return false;
	}
	value = d["value"];

	return true;
}

std::string ExcelParseMgr::parseToProtoType(std::string& sType)
{
	if (!sType.empty())
	{
		sType.erase(0, sType.find_first_not_of(" "));
		sType.erase(sType.find_last_not_of(" ") + 1);
	}
	if (_stricmp(sType.c_str(), "int") == 0
		|| _stricmp(sType.c_str(), "int32") == 0)
	{
		return "int32";
	}
	else if (_stricmp(sType.c_str(), "uint") == 0
		|| _stricmp(sType.c_str(), "uint32") == 0)
	{
		return "uint32";
	}
	else if (_stricmp(sType.c_str(), "int64") == 0)
	{
		return "int64";
	}
	else if (_stricmp(sType.c_str(), "uint64") == 0)
	{
		return "uint64";
	}
	else if (_stricmp(sType.c_str(), "float") == 0)
	{
		return "float";
	}
	else if (_stricmp(sType.c_str(), "double") == 0)
	{
		return "double";
	}
	else if (_stricmp(sType.c_str(), "string") == 0)
	{
		return "string";
	}
	else if (_stricmp(sType.c_str(), "int_array") == 0)
	{
		return "repeated int32";
	}
	else if (_stricmp(sType.c_str(), "int64_array") == 0)
	{
		return "repeated int64";
	}
	else if (_stricmp(sType.c_str(), "uint_array") == 0)
	{
		return "repeated uint32";
	}
	else if (_stricmp(sType.c_str(), "uint64_array") == 0)
	{
		return "repeated uint64";
	}
	else if (_stricmp(sType.c_str(), "string_array") == 0)
	{
		return "repeated string";
	}
	else if (_stricmp(sType.c_str(), "float_array") == 0)
	{
		return "repeated float";
	}
	else if (_stricmp(sType.c_str(), "double_array") == 0)
	{
		return "repeated double";
	}
	else if (_stricmp(sType.c_str(), "object_array") == 0)
	{
		return "object_array";
	}
	else if (_stricmp(sType.c_str(), "json") == 0)
	{
		return "json";
	}
	else
	{
		return "object";
	}

	return sType;
}

void ExcelParseMgr::addProgress(int nProgress)
{
	auto pProgressCtrl = (CProgressCtrl*)(AfxGetMainWnd()->GetDlgItem(IDC_PROGRESS1));
	if (pProgressCtrl == nullptr)
	{
		return;
	}

	int nPos = pProgressCtrl->GetPos();
	if (nPos + nProgress > 10000)
	{
		nProgress -= nPos + nProgress - 10000;
	}

	pProgressCtrl->SetStep(nProgress);
	pProgressCtrl->StepIt();
}

void ExcelParseMgr::updateProgressInfo(std::string sInfo)
{
	auto pProgressCtrl = (CProgressCtrl*)(AfxGetMainWnd()->GetDlgItem(IDC_PROGRESS1));
	if (pProgressCtrl == nullptr)
	{
		return;
	}

	int nPos = pProgressCtrl->GetPos();
	auto nPercent = nPos / 100;

	auto sPercent  = std::to_string(nPercent) + "%";
	AfxGetMainWnd()->SetDlgItemText(IDC_PROGRESS_INFO, sPercent.c_str());
	AfxGetMainWnd()->SetDlgItemText(IDC_PROGRESS_INFO2, sInfo.c_str());
}
