#pragma once
#include "libxl.h"
#include <string>
#include <vector>
#include <map>
#include "rapidjson.h"
#include "document.h"
#include "reader.h"
#include "writer.h"

using namespace libxl;
using namespace rapidjson;

struct ExcelSheetFormat
{
	std::string sName;
	std::vector<std::string> sTitle;
	std::vector<std::string> sType;
	std::vector<std::string> sProtoType;
};

struct ExcelFormat
{
	std::string sFilePath;
	std::string sFileName;

	std::vector<ExcelSheetFormat> vSheetFormat;
};

class ExcelParseMgr
{
public:
	static ExcelParseMgr* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new ExcelParseMgr();
		}
		return m_pInstance;
	}
private:
	ExcelParseMgr();
	~ExcelParseMgr();
public:
	bool IsFinish()const;

	void ParseExcel();

private:
	bool initExcelPath();

	bool parseExcelFormat();
	bool parseSheet(ISheetT<char>* pSheet,ExcelFormat& ef);
	bool parseSheetHead(ISheetT<char>* pSheet,ExcelFormat& ef);

	bool saveToProto();
	bool saveToProto(ExcelFormat& ef,std::string& sData);
	std::string parseToProtoType(std::string& sType);
	// bool saveToSubProto(const std::string& sPBName ,std::string& s,std::string& sData);
	bool saveToSubProto(const std::string& sPBName ,Value& v,std::string& sData,std::string sProtoType);

	bool getNameTypeValue(Value& d, Value& name, Value& type, Value& value);

	bool saveSubProtoIndex(Value& v,const std::string& sParentName, std::string& sData, std::string& sSubData, int index);

	void addProgress(int nProgress);
	void updateProgressInfo(std::string sInfo);
private:
	static ExcelParseMgr* m_pInstance;

	bool m_bFinish;

	std::vector<ExcelFormat> m_vExcelFormats;

	
};