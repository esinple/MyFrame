#include "UserModuleManager.h"
#include "MPLogger.h"
#include "MPTimeTester.h"

UserModuleManager::UserModuleManager(uint32_t nModuleNum, GetUserModuleFuncPtr pFunc)
	: m_vModules(), m_nModuleNum(nModuleNum), m_pFunc(pFunc)
{

}

UserModuleManager::~UserModuleManager()
{
	for (uint32_t i = 0; i < m_nModuleNum; ++i)
	{
		if (m_vModules[i] != nullptr)
		{
			delete m_vModules[i];
			m_vModules[i] = nullptr;
		}
	}
}

bool UserModuleManager::Awake()
{
	m_vModules.resize(m_nModuleNum);
	for (uint32_t i = 0; i < m_nModuleNum; ++i)
	{
		m_vModules[i] = (UserModule*)UserModule::Create((*m_pFunc)(i));
		if (m_vModules[i] == nullptr)
		{
			MP_ERROR("Module Create [%d : %s] Failed!", i, m_pFunc(i));
		}
	}
	for (uint32_t index = 0; index < m_nModuleNum; ++index)
	{
		if (!m_vModules[index]->UserAwake())
		{
			MP_ERROR("Module [%d : %s] Awake Failed!", index, m_pFunc(index));
			return false;
		}
		MP_INFO("Module [%d : %s] Awake Success!", index, m_pFunc(index));
	}
	for (uint32_t index = 0; index < m_nModuleNum; ++index)
	{
		if (!m_vModules[index]->UserAfterAwake())
		{
			MP_ERROR("Module [%d : %s] AfterAwake Failed!", index, m_pFunc(index));
			return false;
		}
		MP_INFO("Module [%d : %s] AfterAwake Success!", index, m_pFunc(index));
	}
	return true;
}

bool UserModuleManager::Execute()
{
	for (uint32_t index = 0; index < m_nModuleNum; ++index)
	{
#ifdef _DEBUG
		meplay::MPTimeTester tester(m_pFunc(index), 100);
#endif
		if (!m_vModules[index]->UserExecute())
		{
			MP_DEBUG("Module [%d : %s] Execute Failed!", index, m_pFunc(index));
			continue;
		}
		//MP_INFO("Module [%d : %s] AfterAwake Success!", index, m_pFunc(index));
	}
	return true;
}
bool UserModuleManager::ShutDown()
{
	for (uint32_t index = 0; index < m_nModuleNum; ++index)
	{
		if (!m_vModules[index]->UserBeforeShutDown())
		{
			MP_ERROR("Module [%d : %s] BeforeShutDown Failed!", index, m_pFunc(index));
			return false;
		}
		MP_INFO("Module [%d : %s] BeforeShutDown Success!", index, m_pFunc(index));
	}
	for (uint32_t index = 0; index < m_nModuleNum; ++index)
	{
		if (!m_vModules[index]->UserShutDown())
		{
			MP_ERROR("Module [%d : %s] ShutDown Failed!", index, m_pFunc(index));
			return false;
		}
		MP_INFO("Module [%d : %s] ShutDown Success!", index, m_pFunc(index));
	}
	return true;
}