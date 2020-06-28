#ifndef TIMER_H
#define TIMER_H

#include<stdint.h>
#include "Atomic.h"
#include "Timestamp.h"
#include "Callbacks.h"

namespace yz
{
	namespace net
	{
		class Timer
		{
		public:
			Timer(const TimerCallback& cb, Timestamp when, double interval)
			 : m_callback(cb), 
			 	 m_expiration(when), 
				 m_interval(interval), 
				 m_repeat(interval > 0.0),
				 m_sequence(s_numCreated.incrementAndGet())
			{
			}
			
			void run() const
			{
				m_callback();
			}
			
			Timestamp expiration() const  { return m_expiration; }
  		bool repeat() const { return m_repeat; }
  		int64_t sequence() const { return m_sequence; }
  		
  		void restart(Timestamp now);
  		
  		static int64_t numCreated() { return s_numCreated.get(); }
			
		private:
			const TimerCallback m_callback;
			Timestamp m_expiration; //��ʱʱ��
			const double m_interval; //�Ƿ��������ģ�Ϊ0��Ϊһ���Զ�ʱ����
			const bool m_repeat; //�Ƿ��ظ�
			const int64_t m_sequence; //��ʱ�����
			
			static AtomicInt64 s_numCreated; //�Ѵ�����ʱ������
		};
	}
}

#endif