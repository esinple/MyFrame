#include "MPThreadPool.h"

using namespace meplay;
MPThreadPool::MPThreadPool()
{
}
MPThreadPool::~MPThreadPool()
{
}
void MPThreadPool::Push(std::shared_ptr<MPThread>& pThread)
{
	m_vThreads.push_back(pThread);
}

std::shared_ptr<MPThread> MPThreadPool::Get(uint8_t nIndex)
{
	if (m_vThreads.size() <= nIndex)
	{
		return nullptr;
	}
	return m_vThreads[nIndex];
}

void MPThreadPool::Start()
{
	for (auto& pThread : m_vThreads)
	{
		pThread->ThreadStart();
	}
}

void MPThreadPool::Final()
{
	for (auto& pThread : m_vThreads)
	{
		pThread->ThreadFinal();
	}
}