#include "ManagerModuleManager.h"
#include "MPLogger.h"
#include "MPTimeTester.h"

ManagerModuleManager::ManagerModuleManager(uint32_t nModuleNum)
	: m_vModules(), m_nModuleNum(nModuleNum)
{
	
}

ManagerModuleManager::~ManagerModuleManager()
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

bool ManagerModuleManager::Awake()
{
	m_vModules.resize(m_nModuleNum);
	for (uint32_t i = 0; i < m_nModuleNum; ++i)
	{
		m_vModules[i] = (ManagerModule*)ManagerModule::Create(i);
		if (m_vModules[i] == nullptr)
		{
			MP_ERROR("Module Create [%d : %s] Failed!", i);
		}
	}

	if (!ModuleRely::TopologicalSort(m_vModules, m_vOrder))
	{
		return false;
	}
	
	for (uint32_t order = 0; order < m_nModuleNum; ++order)
	{
		auto index = m_vOrder[order];
		auto pModule = m_vModules[index];
		if (!pModule->Awake())
		{
			MP_ERROR("Module [%d : %s] Awake Failed!", index, pModule->GetModuleName());
			return false;
		}
		MP_INFO("Module [%d : %s] Awake Success!", index, pModule->GetModuleName());
	}
	MP_SYSTEM("AllModule [%d] Awake Success!",m_nModuleNum);
	for (uint32_t order = 0; order < m_nModuleNum; ++order)
	{
		auto index = m_vOrder[order];
		auto pModule = m_vModules[index];
		if (!pModule->AfterAwake())
		{
			MP_ERROR("Module [%d : %s] AfterAwake Failed!", index, pModule->GetModuleName());
			return false;
		}
		MP_INFO("Module [%d : %s] AfterAwake Success!", index, pModule->GetModuleName());
	}
	MP_SYSTEM("AllModule [%d] AfterAwake Success!", m_nModuleNum);
	return true;
}

bool ManagerModuleManager::Execute()
{
	for (uint32_t index = 0; index < m_nModuleNum; ++index)
	{
		auto pModule = m_vModules[index];
#ifdef _DEBUG
		meplay::MPTimeTester tester(pModule->GetModuleName(),100);
#endif
		if (!pModule->Execute())
		{
			MP_DEBUG("Module [%d : %s] Execute Failed!", index, pModule->GetModuleName());
			continue;
		}
	}

	if (0)
	{
		ShutDown();
	}
	return true;
}
bool ManagerModuleManager::ShutDown()
{
	for (uint32_t order = m_nModuleNum ; order != 0 ; --order)
	{
		auto index = m_vOrder[order - 1];
		auto pModule = m_vModules[index];
		if (!pModule->BeforeShutDown())
		{
			MP_ERROR("Module [%d : %s] BeforeShutDown Failed!", index, pModule->GetModuleName());
			continue;
		}
		MP_INFO("Module [%d : %s] BeforeShutDown Success!", index, pModule->GetModuleName());
	}
	MP_SYSTEM("AllModule [%d] BeforeShutDown Success!", m_nModuleNum);
	for (uint32_t order = m_nModuleNum ; order != 0; --order)
	{
		auto index = m_vOrder[order - 1];
		auto pModule = m_vModules[index];
		if (!m_vModules[index]->ShutDown())
		{
			MP_ERROR("Module [%d : %s] ShutDown Failed!", index, pModule->GetModuleName());
			continue;
		}
		MP_INFO("Module [%d : %s] ShutDown Success!", index, pModule->GetModuleName());
	}
	MP_SYSTEM("AllModule [%d] ShutDown Success!", m_nModuleNum);
	return true;
}