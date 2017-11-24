#pragma once
#include <set>
#include <vector>
#include "MPModule.h"
#include "MPLogger.h"

class ModuleRely
{
public:
	ModuleRely();
	virtual ~ModuleRely();
public:
	void AddParent(int nModuleIndex);
	void DelParent(int nModuleIndex);
	void AddChild(int nModuleIndex);
	void DelChild(int nModuleIndex);
	bool HasParentRely()const;
	const std::set<int>& GetParentRely()const;
	const std::set<int>& GetChildRely()const;

	template<typename T>
	static bool TopologicalSort(
		const std::vector<T>& vModuleRely, 
		std::vector<int>& vOrder,
		const char*(*GetNameFuncPtr)(uint32_t)
	)
	{
		//kahn
		//◊Ó÷’–Ú¡–
		std::vector<int> vQueue;

		for (auto& pModule : vModuleRely)
		{
			if (!pModule->HasParentRely())
			{
				vQueue.emplace_back(pModule->GetModuleType());
			}

			for (auto& nParentIndex : pModule->GetParentRely())
			{
				vModuleRely[nParentIndex]->AddChild(pModule->GetModuleType());
			}
		}

		while (!vQueue.empty())
		{
			auto nIndex = vQueue.back();
			vQueue.pop_back();
			vOrder.emplace_back(nIndex);

			auto& pModule = vModuleRely[nIndex];
			if (pModule == nullptr)
			{
				return false;
			}
			for (auto nChildRely : pModule->GetChildRely())
			{
				auto& pChildModule = vModuleRely[nChildRely];
				pChildModule->DelParent(nIndex);
				if (!pChildModule->HasParentRely())
				{
					vQueue.emplace_back(nChildRely);
				}
			}
		}

		for (auto& pModule : vModuleRely)
		{
			if (pModule->HasParentRely())
			{
				MP_ERROR("Module [%d][%s] Has Circle!", pModule->GetModuleType(), GetNameFuncPtr(pModule->GetModuleType()));
				return false;
			}
		}
		return true;
	}

private:
	std::set<int> m_mParentRely;
	std::set<int> m_mChildRely;
};