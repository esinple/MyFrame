#pragma once
#include <stdint.h>
#include "MPAutoRegister.h"

namespace meplay {
	class MPModule
	{
	public:
		MPModule(uint32_t nModuleType);
		virtual ~MPModule();
	public:
		
	public:
		inline uint32_t GetModuleType()const { return m_nModuleType; }
	private:
		uint32_t m_nModuleType;

		H_AUTO_REGISTER_BASE(MPModule)
	};

}