#include "MPThread.h"
#include "MPLogger.h"

using namespace meplay;

void MPThread::MyFunc(MPThread* pMe)
{
	{
		std::unique_lock<std::mutex> lck(pMe->mtx);
		pMe->cv.wait(lck);
	}

	pMe->Run();

	MP_SYSTEM("New Thread [%s] Run Over!", pMe->m_sName.c_str());
}
MPThread::MPThread(const std::string& name)
	:m_sName(name),m_bFinal(false),m_thread(MyFunc,this)
{
	MP_SYSTEM("New Thread [%s] Create!",m_sName.c_str());
}

MPThread::~MPThread()
{
}

MPThread::MPThread(MPThread&& t)
	: m_sName(t.m_sName), m_bFinal(std::move(t.m_bFinal)),m_thread(std::move(t.m_thread))
{
}

void MPThread::ThreadStart()
{
	MP_SYSTEM("Thread [%s] Start!",m_sName.c_str());
	cv.notify_all();
}

void MPThread::ThreadFinal()
{
	m_bFinal = true;
	cv.notify_all();
	m_thread.join();
	MP_SYSTEM("Thread [%s] Destroy!", m_sName.c_str());
}

bool MPThread::IsThreadFinal()const
{
	return m_bFinal;
}

void MPThread::Sleep(uint32_t sec)
{
	std::this_thread::sleep_for(std::chrono::seconds(sec));
}

void MPThread::MSleep(uint32_t msec)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(msec));
}