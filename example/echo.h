#ifndef EXAMPLES_ECHO_ECHO_H
#define EXAMPLES_ECHO_ECHO_H

#include "TcpServer.h"

class EchoServer
{
public:
	EchoServer(yz::net::EventLoop* loop, const yz::net::InetAddress& listenAddr);
	
	void start();
	
private:
	void onConnection(const yz::net::TcpConnectionPtr& conn);
		
	void onMessage(const yz::net::TcpConnectionPtr& conn, yz::net::Buffer* buf, yz::Timestamp time_);
	
	yz::net::EventLoop* m_loop;
	yz::net::TcpServer m_server;
};

#endif