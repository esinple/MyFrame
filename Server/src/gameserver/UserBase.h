#pragma once
#include "UserModule.h"

class UserBase final : public UserModule
{
public:
	UserBase();
	~UserBase();
public:
	virtual bool UserAwake()override;
	virtual bool UserAfterAwake()override;
	virtual bool UserExecute()override;
	virtual bool UserBeforeShutDown()override;
	virtual bool UserShutDown()override;
private:
};

GAME_USER_MODULE_REG(eGameUser_Base, UserBase);