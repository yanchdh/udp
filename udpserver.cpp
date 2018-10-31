/*
	udpserver.cpp
*/

# include "udp.hpp"


int main(int argc, char* argv[])
{
	UDPServer server("0.0.0.0", 3333);
	server.loop();

	return 0;
}