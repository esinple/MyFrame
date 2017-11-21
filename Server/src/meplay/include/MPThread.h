#pragma once

#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>

namespace meplay {

	class MPThread
	{
	public:
		MPThread(const std::string& name);
		MPThread(const MPThread&) = delete;
		MPThread(MPThread&& t);
		virtual ~MPThread();
	public:
		virtual void Run() = 0;
	public:
		void ThreadStart();

		void ThreadFinal();

		bool IsThreadFinal()const;

		static void MyFunc(MPThread* pMe);

		void Sleep(uint32_t sec);
		void MSleep(uint32_t msec);

	private:
		const std::string m_sName;

		bool m_bFinal;

	protected:
		std::mutex mtx;
		std::condition_variable cv;
		std::thread m_thread;
	};
}