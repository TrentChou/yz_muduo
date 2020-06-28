#ifndef EVENTLOOPTHREAD_H
#define EVENTLOOPTHREAD_H

#include <functional>
#include "Condition.h"
#include "Mutex.h"
#include "thread.h"
#include "EventLoop.h"

namespace yz
{
	namespace net
	{
		class EventLoopThread
		{
		public:
			typedef std::function<void(EventLoop*)> ThreadInitCallback;

  		EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback());
  		~EventLoopThread();
  		EventLoop* startLoop(); //�����߳�
  
		private:
			void threadFunc(); //�̺߳���

  		EventLoop* m_loop; //loopָ��ָ��һ��EventLoop����
  		bool m_exiting;
  		Thread m_thread;
  		MutexLock m_mutex;
  		Condition m_cond;
  		ThreadInitCallback m_callback; //���ص���Ϊ�գ��򽫻���EventLoop::loop()�¼�ѭ��֮ǰ������
		};
	}
}

#endif