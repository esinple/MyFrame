#pragma once

#include <vector>
#include "libxl.h"

using namespace libxl;
struct ExcelSheetFormat;
struct ExcelFormat;
class XmlMgr
{
public:
	static XmlMgr* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new XmlMgr();
		}
		return m_pInstance;
	}
private:
	XmlMgr();
	~XmlMgr();
public:
	bool WriteToXml(ISheetT<char>* pSheet,std::string& sTableName,ExcelSheetFormat& ef);
private:
	bool parseData(std::string& sValue,ISheetT<char>* pSheet,uint32_t nRow,uint32_t nCol,std::string& sType);
	void testRead();
private:
	static XmlMgr* m_pInstance;
};