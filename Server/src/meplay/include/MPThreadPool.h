#pragma once

#include "MPThread.h"

namespace meplay
{
	class MPThreadPool final
	{
	public:
		MPThreadPool();
		~MPThreadPool();
	public:
		void Push(std::shared_ptr<MPThread>& pThread);
		std::shared_ptr<MPThread> Get(uint8_t nIndex);

		void Start();

		void Final();

	private:
		std::vector<std::shared_ptr<MPThread>> m_vThreads;
	};
}