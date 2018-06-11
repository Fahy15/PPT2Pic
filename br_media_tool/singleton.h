#pragma once
//#include "../../HMCCommonLib/GSCommon.h"
//#include "../../HMCCommonLib/ISystemLayInterface.h"
#include "common.h"

/**
* @file     Singleton.h
* @brief	单态类模板,线程安全

* History
*  2012/8/17 由gaowg创建
*/

#define DECLARE_SINGLETON(className) \
	 private: \
	 friend Singleton<className>;

template<class T>
class Singleton
{
public:
	Singleton(void){}
	static T* GetInstance()
	{
		//锁外一次判断
		if (m_Instance == NULL)
		{
			CGSAutoMutex	AutoLock(&m_CsSingleton);
			if(m_Instance == NULL)
				m_Instance = new T;
		}
		return m_Instance;
	}
	static void Release()
	{
		if(m_Instance != NULL)
		{
			delete m_Instance;
			m_Instance = NULL;
		}
	}

	friend T;
private:	
	static	CGSMutex	m_CsSingleton;	// 单例锁
	static T* m_Instance;
};

template<class T>
T* Singleton<T>::m_Instance = 0; 

template<class T>
CGSMutex	Singleton<T>::m_CsSingleton;

#define  INSTANCE(X) (X::GetInstance())