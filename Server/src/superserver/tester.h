#pragma once
#include "MPThread.h"
#include <map>
#include <string>

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

class HttpClient
{
public:
	HttpClient();
	~HttpClient();
public:
	void TestCB(const char* str, int len);
private:
};