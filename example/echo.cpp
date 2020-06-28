#include <functional>
#include "Timestamp.h"
#include "echo.h"
#include "Logging.h"

EchoServer::EchoServer(yz::net::EventLoop* loop, const yz::net::InetAddress& listenAddr)
	:m_loop(loop),
		m_server(m_loop, listenAddr, "EchoServer")
{
	m_server.setConnectionCallback(std::bind(&EchoServer::onConnection, this, std::placeholders::_1));
	m_server.setMessageCallback(std::bind(&EchoServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}
	
void EchoServer::start()
{
	LOG_INFO << "EchoServer Start " ;
	m_server.start();
}
	
void EchoServer::onConnection(const yz::net::TcpConnectionPtr& conn)
{
	LOG_INFO << "EchoServer - " << conn->peerAddress().toIpPort() << " -> "
           << conn->localAddress().toIpPort() << " is "
           << (conn->connected() ? "UP" : "DOWN");
}
		
void EchoServer::onMessage(const yz::net::TcpConnectionPtr& conn, yz::net::Buffer* buf, yz::Timestamp time_)
{
	yz::string msg(buf->retrieveAllAsString());
	LOG_INFO << conn->name() << " echo " << msg.size() << " bytes, " << "data received at " << time_.toString();;
	
	conn->send(msg);
}