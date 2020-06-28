/*
-------------------------------
MutexLock
-------------------------------
muetx_:pthread_mutex_t;
holder_:pid_t;
-------------------------------
<<create>>MutexLock
<<destrou>>MutexLock
isLockedByThisThread():bool
assertLocked()
lock()
unlock()
getPthreadMutex()pthread_mutex_t*
-------------------------------
*/

#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>
#include <assert.h>
#include <sys/types.h>
#include "CurrentThread.h"
#include "thread.h"

class MutexLock
{
public:
	MutexLock():m_holder(0)
	{
		int ret = pthread_mutex_init(&m_mutex, NULL);
		assert(ret == 0); 
		(void) ret;
	}
	
	~MutexLock()
	{
		assert(m_holder == 0);
		int ret = pthread_mutex_destroy(&m_mutex);
		assert(ret == 0);
		(void)ret;
	}
	
	bool isLockedByThisThread()
	{
		return m_holder == yz::CurrentThread::tid();
	}
	
	void assertLocked()
	{
		assert(isLockedByThisThread());
	}
	
	void lock()
	{
		pthread_mutex_lock(&m_mutex);
		m_holder = yz::CurrentThread::tid();
	}
	
	void unlock()
	{
		m_holder = 0;
		pthread_mutex_unlock(&m_mutex);
	}
	
	pthread_mutex_t* getpthreadMutex()
	{
		return &m_mutex;
	}
	
private:
	pthread_mutex_t m_mutex;
	pid_t m_holder;
};

//自由获取，占有或释放RAII
 class MutexLockGuard
{
public:
	explicit MutexLockGuard(MutexLock& mutex):m_mutex(mutex)
	{
		m_mutex.lock();
	}
	
	~MutexLockGuard()
	{
		m_mutex.unlock();
	}
	
private:
	MutexLock& m_mutex; //just ref
};

//don't allow define a no_name object 
#define MutexLockGuard(x) error "Missing guard object name" 

#endif