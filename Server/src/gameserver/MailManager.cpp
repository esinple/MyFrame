#include "MailManager.h"

MailManager::MailManager() : ManagerModule(eGameMgr_Mail)
{
}

MailManager::~MailManager()
{
}

bool MailManager::Awake()
{
	return true;
}

bool MailManager::AfterAwake()
{
	return true;
}

bool MailManager::Execute()
{
	return true;
}

bool MailManager::BeforeShutDown()
{
	return true;
}

bool MailManager::ShutDown()
{
	return true;
}