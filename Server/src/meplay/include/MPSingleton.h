#ifndef _MP_SINGLETON_H_
#define	_MP_SINGLETON_H_

#define MP_SINGLETON_DECLARE(CLASS_NAME) \
	private: \
		static CLASS_NAME* m_pInstance; \
	public: \
		static CLASS_NAME* GetInstance() \
		{ \
			if (m_pInstance == nullptr) \
			{ \
				m_pInstance = new CLASS_NAME(); \
			} \
			return m_pInstance; \
		} \
		static void DestroyInstance() \
		{ \
			if(m_pInstance != nullptr) \
			{ \
				delete m_pInstance; \
				m_pInstance = nullptr; \
			} \
		} \
	private: \
		CLASS_NAME(); \
		~CLASS_NAME(); \
		CLASS_NAME(const CLASS_NAME&) = delete; \
		const CLASS_NAME& operator=(const CLASS_NAME&) = delete;

#define MP_SINGLETON_IMPLEMENT(CLASS_NAME) \
	CLASS_NAME* CLASS_NAME::m_pInstance = new CLASS_NAME();

#endif // !_MP_SINGLETON_H_
