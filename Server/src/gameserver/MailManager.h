#pragma once

#include "GameCommDef.h"

class MailManager : public ManagerModule
{
public:
	MailManager();
	~MailManager();
public:
	virtual bool Awake()override;
	virtual bool AfterAwake()override;
	virtual bool Execute()override;
	virtual bool BeforeShutDown()override;
	virtual bool ShutDown()override;
private:
	
};