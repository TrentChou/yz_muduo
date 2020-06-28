#include "EventLoop.h"
#include "echo.h"
#include "Logging.h"

int main(int argc, char* args[])
{
	LOG_INFO << "pid = " << getpid();
	yz::net::EventLoop loop;
	yz::net::InetAddress listenAddr(2007);
	EchoServer server(&loop, listenAddr);
	server.start();
	loop.loop();
	
	return 0;
}