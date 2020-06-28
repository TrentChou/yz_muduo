#ifndef SINGLETON_H
#define SINGLETON_H

#include<pthread.h>
#include<stdlib.h>

namespace yz
{
	template<typename T>
	class Singleton
	{
	public:
		static T& instance()
		{
			pthread_once(&m_ponce, &Singleton::init);
			return *m_value;
		}
		
	private:
		Singleton();
		~Singleton();
		
		static void init()
		{
			m_value = new T();
			::atexit(destroy);
		}
		
		static void destroy()
		{   
			typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
			delete m_value;
		}
		
	private:
		static pthread_once_t m_ponce;
		static T* m_value;
	};
	
	template<typename T>
	pthread_once_t Singleton<T>::m_ponce = PTHREAD_ONCE_INIT;
		
	template<typename T>
	T* Singleton<T>::m_value = NULL;
	
}

#endif