#include "ModuleRely.h"

ModuleRely::ModuleRely()
{
}

ModuleRely::~ModuleRely()
{
}

void ModuleRely::AddParent(int nModuleIndex)
{
	m_mParentRely.emplace(nModuleIndex);
}

void ModuleRely::DelParent(int nModuleIndex)
{
	m_mParentRely.erase(nModuleIndex);
}

void ModuleRely::AddChild(int nModuleIndex)
{
	m_mChildRely.emplace(nModuleIndex);
}

void ModuleRely::DelChild(int nModuleIndex)
{
	m_mChildRely.erase(nModuleIndex);
}

bool ModuleRely::HasParentRely()const
{
	return !m_mParentRely.empty();
}

const std::set<int>& ModuleRely::GetParentRely()const
{
	return m_mParentRely;
}
const std::set<int>& ModuleRely::GetChildRely()const
{
	return m_mChildRely;
}