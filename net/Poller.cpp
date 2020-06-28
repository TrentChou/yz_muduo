#include"Poller.h"

using namespace yz;
using namespace yz::net;
	
Poller::Poller(EventLoop* loop):m_ownerLoop(loop)
{
}

Poller::~Poller()
{
}