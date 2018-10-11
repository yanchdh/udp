/*
	udpserver.cpp
*/

# include "udp.hpp"


int main(int argc, char* argv[])
{
	UDPServer server("127.0.0.1", 3333);
	server.loop();

	return 0;
}