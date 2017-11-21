#include "TMManager.h"
#include "SuperManagerModuleEnum.h"
#include "SuperNetProxy.h"

CPP_SUPER_MANAGER_REG(TMManager);

TMManager::TMManager() 
	: ManagerModule(eSuperManagerModule_Test)
{
}

TMManager::~TMManager()
{
}
bool TMManager::Awake()
{
	return true;
}
bool TMManager::AfterAwake()
{
	return true;
}
bool TMManager::Execute()
{
	return true;
}
bool TMManager::BeforeShutDown()
{
	return true;
}
bool TMManager::ShutDown()
{
	return true;
}