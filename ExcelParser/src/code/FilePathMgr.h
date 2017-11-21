#pragma once
#include <filesystem>

class FilePathMgr
{
public:
	static FilePathMgr* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new FilePathMgr();
		}
		return m_pInstance;
	}
private:
	FilePathMgr();
	~FilePathMgr();
public:
	void SetOpenInPath(const std::string& sPath);
	void SetOpenOutPath(const std::string& sPath);

	const std::string& GetOpenInPath()const;
	const std::string& GetOpenOutPath()const;
private:
	static FilePathMgr* m_pInstance;
	std::string m_sPathIn;
	std::string m_sPathOut;
};