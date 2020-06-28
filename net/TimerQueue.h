#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H

#include<iostream>
#include<set>
#include<vector>
#include "Mutex.h"
#include "Timestamp.h"
#include "Callbacks.h"
#include "Channel.h"

namespace yz
{
	namespace net
	{
		class EventLoop;
		class Timer;
		class TimerId;
		
		class TimerQueue
		{
		public:
			TimerQueue(EventLoop* loop);
			~TimerQueue();
			
			TimerId addTimer(const TimerCallback& cb, Timestamp when, double interval);
			
			void cancel(TimerId timerId);
			
		private:
			// FIXME: use unique_ptr<Timer> instead of raw pointers.
			typedef std::pair<Timestamp, Timer*> Entry;
			typedef std::set<Entry> TimerList;
			typedef std::pair<Timer*, int64_t> ActiveTimer;
			typedef std::set<ActiveTimer> ActiveTimerSet;
				
			void addTimerInLoop(Timer* timer);
			void cancelInLoop(TimerId timerId);
			
			void handleRead();
			
			std::vector<Entry> getExpired(Timestamp now);
			void reset(const std::vector<Entry>& expired, Timestamp now);
				
			bool insert(Timer* timer);
			
			EventLoop* m_loop; //����EventLoop
			const int m_timerfd; //ʱ���ļ�������
			Channel m_timerfdChannel; //
			TimerList m_timers; //������ʱ��������б�
			
			ActiveTimerSet m_activeTimers; //�������ַ����
			bool m_callingExpiredTimers;
			ActiveTimerSet m_cancelingTimers; //������Ǳ�ȡ���Ķ�ʱ��
		};
	}
}

#endif