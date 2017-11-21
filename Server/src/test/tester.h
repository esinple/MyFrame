#pragma once
#include "MPThread.h"

class TestThreader : public meplay::MPThread
{
public:
	TestThreader();
	~TestThreader();
public:
	virtual void Run() override;
private:
};

class Tester
{
public:
	Tester();
	~Tester();
public:
	void TestDB();
	
	void TestTimer();
private:
};