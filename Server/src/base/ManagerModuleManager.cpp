#include "ManagerModuleManager.h"
#include "MPLogger.h"
#include "MPTimeTester.h"

ManagerModuleManager::ManagerModuleManager(uint32_t nModuleNum, GetNameFuncPtr pFunc)
	: m_vModules(), m_nModuleNum(nModuleNum), m_pFunc(pFunc)
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
		m_vModules[i] = (ManagerModule*)ManagerModule::Create((*m_pFunc)(i));
		if (m_vModules[i] == nullptr)
		{
			MP_ERROR("Module Create [%d : %s] Failed!", i, m_pFunc(i));
		}
	}

	if (!ModuleRely::TopologicalSort(m_vModules, m_vOrder, m_pFunc))
	{
		return false;
	}
	
	for (uint32_t order = 0; order < m_nModuleNum; ++order)
	{
		auto index = m_vOrder[order];
		if (!m_vModules[index]->Awake())
		{
			MP_ERROR("Module [%d : %s] Awake Failed!", index, m_pFunc(index));
			return false;
		}
		MP_INFO("Module [%d : %s] Awake Success!", index, m_pFunc(index));
	}
	MP_SYSTEM("AllModule [%d] Awake Success!",m_nModuleNum);
	for (uint32_t order = 0; order < m_nModuleNum; ++order)
	{
		auto index = m_vOrder[order];
		if (!m_vModules[index]->AfterAwake())
		{
			MP_ERROR("Module [%d : %s] AfterAwake Failed!", index, m_pFunc(index));
			return false;
		}
		MP_INFO("Module [%d : %s] AfterAwake Success!", index, m_pFunc(index));
	}
	MP_SYSTEM("AllModule [%d] AfterAwake Success!", m_nModuleNum);
	return true;
}

bool ManagerModuleManager::Execute()
{
	for (uint32_t index = 0; index < m_nModuleNum; ++index)
	{
#ifdef _DEBUG
		meplay::MPTimeTester tester(m_pFunc(index),100);
#endif
		if (!m_vModules[index]->Execute())
		{
			MP_DEBUG("Module [%d : %s] Execute Failed!", index, m_pFunc(index));
			continue;
		}
		//MP_INFO("Module [%d : %s] AfterAwake Success!", index, m_pFunc(index));
	}
	return true;
}
bool ManagerModuleManager::ShutDown()
{
	for (uint32_t order = m_nModuleNum - 1; order > 0 ; --order)
	{
		auto index = m_vOrder[order];
		if (!m_vModules[index]->BeforeShutDown())
		{
			MP_ERROR("Module [%d : %s] BeforeShutDown Failed!", index, m_pFunc(index));
			continue;
			//return false;
		}
		MP_INFO("Module [%d : %s] BeforeShutDown Success!", index, m_pFunc(index));
	}
	MP_SYSTEM("AllModule [%d] BeforeShutDown Success!", m_nModuleNum);
	for (uint32_t order = m_nModuleNum - 1; order > 0; --order)
	{
		auto index = m_vOrder[order];
		if (!m_vModules[index]->ShutDown())
		{
			MP_ERROR("Module [%d : %s] ShutDown Failed!", index, m_pFunc(index));
			continue;
		}
		MP_INFO("Module [%d : %s] ShutDown Success!", index, m_pFunc(index));
	}
	MP_SYSTEM("AllModule [%d] ShutDown Success!", m_nModuleNum);
	return true;
}

bool ManagerModuleManager::topologicalSort()
{
	//kahn
	//◊Ó÷’–Ú¡–
	std::vector<int> vQueue;

	for (auto& pModule : m_vModules)
	{
		if (!pModule->HasParentRely())
		{
			vQueue.emplace_back(pModule->GetModuleType());
		}

		for (auto& nParentIndex : pModule->GetParentRely())
		{
			m_vModules[nParentIndex]->AddChild(pModule->GetModuleType());
		}
	}

	while (!vQueue.empty())
	{
		auto nIndex = vQueue.back();
		vQueue.pop_back();
		m_vOrder.emplace_back(nIndex);

		auto& pModule = m_vModules[nIndex];
		if (pModule == nullptr)
		{
			return false;
		}
		for (auto nChildRely : pModule->GetChildRely())
		{
			auto& pChildModule = m_vModules[nChildRely];
			pChildModule->DelParent(nIndex);
			if (!pChildModule->HasParentRely())
			{
				vQueue.emplace_back(nChildRely);
			}
		}
	}

	for (auto& pModule : m_vModules)
	{
		if (pModule->HasParentRely())
		{
			MP_ERROR("Module [%d][%s] Has Circle!", pModule->GetModuleType(), m_pFunc(pModule->GetModuleType()));
			return false;
		}
	}

	return true;
}