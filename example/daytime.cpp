#include <functional>
#include "daytime.h"
#include "Logging.h"

DaytimeServer::DaytimeServer(yz::net::EventLoop* loop, const yz::net::InetAddress& listenAddr)
	: m_loop(loop),
		m_server(m_loop, listenAddr, "DayTimeServer")
{
	m_server.setConnectionCallback(std::bind(&DaytimeServer::OnConnection, this, std::placeholders::_1));
	m_server.setMessageCallback(std::bind(&DaytimeServer::OnMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}
	
void DaytimeServer::start()
{
	m_server.start();
}
	
void DaytimeServer::OnConnection(const yz::net::TcpConnectionPtr& conn)
{
	LOG_INFO << "DaytimeServer - " << conn->peerAddress().toIpPort() << " -> "
           << conn->localAddress().toIpPort() << " is "
           << (conn->connected() ? "UP" : "DOWN");
  if(conn->connected())
  {
  	conn->send(yz::Timestamp::now().toFormattedString() + "\n");
  	conn->shutdown();
  }
}

void DaytimeServer::OnMessage(const yz::net::TcpConnectionPtr& conn, yz::net::Buffer* buf, yz::Timestamp time_)
{
	yz::string msg(buf->retrieveAllAsString());
	LOG_INFO << conn->name() << " discards " << msg.size() << " bytes received at " << time_.toString();
}