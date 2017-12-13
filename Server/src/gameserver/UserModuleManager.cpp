#include "UserModuleManager.h"
#include "MPLogger.h"
#include "MPTimeTester.h"
#include "GameCommDef.h"

std::vector<int> UserModuleManager::m_vOrder;

#define USER_MODULE_NAME(i) MPModuleFactory::GetInstance()->GetName(eGameUserModule,i).data()

UserModuleManager::UserModuleManager(uint32_t nModuleNum)
	: m_vModules(), m_nModuleNum(nModuleNum)
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
		m_vModules[i] = (UserModule*)MPModuleFactory::GetInstance()->Create(eGameUserModule,i);
		if (m_vModules[i] == nullptr)
		{
			MP_ERROR("Module Create [%d : %s] Failed!", i, USER_MODULE_NAME(i));
		}
	}
	//info test;

	if (m_vOrder.empty())
	{
		if (!ModuleRely::TopologicalSort(m_vModules, m_vOrder))
		{
			return false;
		}
		//≥ı ºªØº”‘ÿÀ≥–Ú
	}

	for (uint32_t order = 0; order < m_nModuleNum; ++order)
	{
		auto index = m_vOrder[order];
		auto pModule = m_vModules[index];
		if (!pModule->UserAwake())
		{
			MP_ERROR("Module [%d : %s] Awake Failed!", index, USER_MODULE_NAME(index));
			return false;
		}
		MP_DEBUG("Module [%d : %s] Awake Success!", index, USER_MODULE_NAME(index));
	}
	for (uint32_t order = 0; order < m_nModuleNum; ++order)
	{
		auto index = m_vOrder[order];
		auto pModule = m_vModules[index];
		if (!pModule->UserAfterAwake())
		{
			MP_ERROR("Module [%d : %s] AfterAwake Failed!", index, USER_MODULE_NAME(index));
			return false;
		}
		MP_DEBUG("Module [%d : %s] AfterAwake Success!", index, USER_MODULE_NAME(index));
	}
	return true;
}

bool UserModuleManager::Execute()
{
	for (uint32_t index = 0; index < m_nModuleNum; ++index)
	{
		auto pModule = m_vModules[index];
#ifdef _DEBUG
		meplay::MPTimeTester tester(USER_MODULE_NAME(index), 100);
#endif
		if (!m_vModules[index]->UserExecute())
		{
			MP_DEBUG("Module [%d : %s] Execute Failed!", index, USER_MODULE_NAME(index));
			continue;
		}
	}
	return true;
}
bool UserModuleManager::ShutDown()
{
	for (uint32_t order = m_nModuleNum ; order != 0; --order)
	{
		auto index = m_vOrder[order - 1];
		auto pModule = m_vModules[index];
		if (!pModule->UserBeforeShutDown())
		{
			MP_ERROR("Module [%d : %s] BeforeShutDown Failed!", index, USER_MODULE_NAME(index));
			return false;
		}
		MP_DEBUG("Module [%d : %s] BeforeShutDown Success!", index, USER_MODULE_NAME(index));
	}
	for (uint32_t order = m_nModuleNum; order != 0; --order)
	{
		auto index = m_vOrder[order - 1];
		auto pModule = m_vModules[index];
		if (!pModule->UserShutDown())
		{
			MP_ERROR("Module [%d : %s] ShutDown Failed!", index, USER_MODULE_NAME(index));
			return false;
		}
		MP_DEBUG("Module [%d : %s] ShutDown Success!", index, USER_MODULE_NAME(index));
	}
	return true;
}

std::vector<UserModule*>& UserModuleManager::GetAllModules()
{
	return m_vModules;
}