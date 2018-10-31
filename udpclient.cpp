/*
	udpclient.cpp
*/

# include <stdlib.h>
# include "udp.hpp"


int main(int argc, char* argv[])
{
	const char* host = "127.0.0.1";
	int port = 3333;
	if (argc > 2)
	{
		host = argv[1];
		port = atoi(argv[2]);
	}

	UDPClient client(host, port);
	client.loop();

	/*
	tcpdump -i lo -n -x port 3333
	21:58:00.572180 IP 127.0.0.1.50168 > 127.0.0.1.3333: UDP, length 13
	0x0000:  4500 0029 aedb 4000 4011 8de6 7f00 0001
	0x0010:  7f00 0001 c3f8 0d05 0015 fe28 6865 6c6c
	0x0020:  6f20 776f 726c 6420 30
	*/

	return 0;
}