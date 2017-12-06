#pragma once
#include "MPTime.h"

class Mail
{
public:
	Mail();
	virtual ~Mail();

private:
	meplay::MPTime m_CreateTime;
	meplay::MPTime m_ReadTime;

};