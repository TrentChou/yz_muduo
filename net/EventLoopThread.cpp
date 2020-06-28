#include "EventLoopThread.h"
#include "EventLoop.h"

using namespace yz;
using namespace yz::net;
	
EventLoopThread::EventLoopThread(const ThreadInitCallback& cb)
  : m_loop(NULL),
    m_exiting(false),
    m_thread(std::bind(&EventLoopThread::threadFunc, this)),
    m_mutex(),
    m_cond(m_mutex),
    m_callback(cb)
{
}

EventLoopThread::~EventLoopThread()
{
  m_exiting = true;
  m_loop->quit(); //��ֹI/O�߳�
  m_thread.join();
}

EventLoop* EventLoopThread::startLoop()
{
  assert(!m_thread.started());
  m_thread.start();

  {
    MutexLockGuard lock(m_mutex);
    while (m_loop == NULL)
    {
    	//�ȴ�start�е�threadfunc����EventLoop����ɹ���Ż᷵��
      m_cond.wait(); 
    }
  }

  return m_loop;
}

void EventLoopThread::threadFunc()
{
	//ջ���󣬵��˺����˳�����ζ�Ŵ�I/O�̵߳���ֹ
  EventLoop loop;

  if (m_callback)
  {
    m_callback(&loop); //�����ں��̱߳��ش洢���ݽ�ϣ���߷������˵Ĳ�������
  }

  {
    MutexLockGuard lock(m_mutex);
    m_loop = &loop;
    m_cond.notify();
  }

  loop.loop();
  //assert(exiting_);
}