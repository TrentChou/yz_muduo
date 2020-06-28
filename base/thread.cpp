/*
----------------------------------
Thread
------------------------------------------------+
started_:bool;
pthreadId_:pthread_t;
tid_:pid_t;
func_:ThreadFunc;
name:string;
static :numCreated:AtomicInt32;
-------------------------------------------------+
<<create>>Thread(const ThreadFunc&, string);
<<destroy>>Thread();
start():void
join():int
started():bool
tid():pid_t
name():const string&
static numCreated():int
static startThread(thread:void)void* //entry
    +-runInThread():void
        +-Func()
--------------------------------------------------+
*/

#include "CurrentThread.h"
#include "thread.h"
#include "Logging.h"

namespace yz
{
	namespace CurrentThread
	{
  	__thread int t_cachedTid = 0;
  	__thread char t_tidString[32];
  	__thread const char* t_threadName = "unknown";
  	const bool sameType = std::is_same<int, pid_t>::value;
  	static_assert(sameType, "sameType is false!!!");
	}

	namespace detail
	{

		pid_t gettid()
		{
  		return static_cast<pid_t>(::syscall(SYS_gettid));
		}

		void afterFork()
		{
  		yz::CurrentThread::t_cachedTid = 0;
  		yz::CurrentThread::t_threadName = "main";
  		CurrentThread::tid();
  		// no need to call pthread_atfork(NULL, NULL, &afterFork);
		}

		class ThreadNameInitializer
		{
 		public:
  		ThreadNameInitializer()
  		{
    		yz::CurrentThread::t_threadName = "main";
    		CurrentThread::tid();
    		pthread_atfork(NULL, NULL, &afterFork);//线程在执行fork以后，成为了新的进程的父进程
  		}
		};

		ThreadNameInitializer init;
	}
}

using namespace yz;

void CurrentThread::cacheTid()
{
  if (t_cachedTid == 0)
  {
    t_cachedTid = detail::gettid();
    int n = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
    assert(n == 6); (void) n;
  }
}

bool CurrentThread::isMainThread()
{
  return tid() == ::getpid();
}

AtomicInt32 Thread::s_numCreated;

//using namespace yz;

Thread::Thread(const threadfunc_& run_func, const std::string& n)
					:m_started(false), m_pthreadId(0), m_tid(0), m_name(n), m_threadfunc(run_func) 
{
	s_numCreated.increment();//threadnum+1
}

Thread::~Thread()
{
	//m_started = false;
}

void* Thread::startThread(void* thread_obj)
{
	Thread* thread_ = static_cast<Thread*>(thread_obj);
	thread_->runInThread();
	return NULL;
}
 
void Thread::runInThread()
{
	m_tid = CurrentThread::tid();
	yz::CurrentThread::t_threadName = m_name.empty() ? "muduoThread" : m_name.c_str();
	try
	{
		m_threadfunc();
	}
	catch(...)
	{
		throw std::exception();
	}
}

void Thread::start()
{
	assert(!m_started);
	m_started = true;
	
	errno = pthread_create(&m_pthreadId, NULL, startThread, this);
	if(0 != errno)
	{
		//log( LOG_ERR, __FILE__, __LINE__, "Failed in pthread_create\n");
		LOG_SYSFATAL << "Failed in pthread_create";
	}
}

int Thread::join()
{
	assert(m_started);
	return pthread_join(m_pthreadId, NULL);
}