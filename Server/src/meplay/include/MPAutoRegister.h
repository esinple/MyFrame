#pragma once
#include <string>
#include <map>

#define DECLEAR_AUTO_REGISTER_BASE(ClassBase) \
public: \
	typedef ClassBase *(*ClassGen)(); /* 声明函数指针*/ \
protected: \
	static std::map<std::string, ClassGen>& ClassBaseMap() \
	{ \
		static std::map<std::string, ClassGen> m_mClassBaseMap; /* 存储子类信息 */ \
		return m_mClassBaseMap; \
	} \
public: \
	static ClassBase *Create(const std::string& class_name) \
	{ \
	auto iter = ClassBaseMap().find(class_name); \
	if (ClassBaseMap().end() != iter) \
		{ \
			return ((*iter).second)(); \
		} \
		return nullptr; \
	} \
protected: \
	static void Register(const std::string &class_name, ClassGen class_gen) /* 注册函数 */ \
	{ \
		ClassBaseMap().insert(make_pair(class_name, class_gen)); \
	} 

#define H_AUTO_REGISTER_BASE(BASE) DECLEAR_AUTO_REGISTER_BASE(BASE)

// 声明子类
#define DECLEAR_AUTO_REGISTER_SUB(BaseClass,ClassSub) \
public: \
	struct ClassSub##Register /* 用于启动注册的结构体 */ \
	{ \
		ClassSub##Register() /*构造函数中注册子类*/ \
		{ \
			static bool bRegistered = false;  \
			if(!bRegistered) \
			{ \
				meplay::MPModule::Register(#ClassSub, ClassSub::Create); \
				bRegistered = true; \
			} \
		} \
	} ; \
	static MPModule* Create() \
	{ \
		return (BaseClass*)(new ClassSub()); \
	} \
	static struct ClassSub##Register m_t##ClassSub##Register;

// 实现子类
#define IMPLEMENT_AUTO_REGISTER_SUB(ClassSub) \
	static ClassSub::ClassSub##Register m_t##ClassSub##Register;

#define H_AUTO_REGISTER_SUB(BASE,SUB) DECLEAR_AUTO_REGISTER_SUB(BASE,SUB)
#define CPP_AUTO_REGISTER_SUB(SUB) IMPLEMENT_AUTO_REGISTER_SUB(SUB)
