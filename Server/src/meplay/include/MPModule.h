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
		const char * GetModuleName()const { return ClassNameMap().find(m_nModuleType)->second.data(); }
	private:
		uint32_t m_nModuleType;

		H_AUTO_REGISTER_BASE(MPModule)
	};

}