#ifndef __DAYTIME_H_
#define __DAYTIME_H_

#include "TcpServer.h"
#include "EventLoop.h"

class DaytimeServer
{
public:
	DaytimeServer(yz::net::EventLoop* loop, const yz::net::InetAddress& listenAddr);
	
	void start();
	
private:
	void OnConnection(const yz::net::TcpConnectionPtr& conn);
	
	/*δʹ��*/
	void OnMessage(const yz::net::TcpConnectionPtr& conn, yz::net::Buffer* buf, yz::Timestamp time_);
	
private:
	yz::net::EventLoop* m_loop;
	yz::net::TcpServer m_server;
};

#endif