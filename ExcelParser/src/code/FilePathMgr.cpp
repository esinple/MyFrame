#include "FilePathMgr.h"

FilePathMgr* FilePathMgr::m_pInstance = new FilePathMgr();

FilePathMgr::FilePathMgr()
{
}

FilePathMgr::~FilePathMgr()
{
}

void FilePathMgr::SetOpenInPath(const std::string& sPath)
{
	m_sPathIn = sPath;
}

void FilePathMgr::SetOpenOutPath(const std::string& sPath)
{
	m_sPathOut = sPath;
}

const std::string& FilePathMgr::GetOpenInPath()const
{
	return m_sPathIn;
}
const std::string& FilePathMgr::GetOpenOutPath()const
{
	return m_sPathOut;
}