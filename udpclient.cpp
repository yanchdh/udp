/*
	udpserver.cpp
*/

# include "udp.hpp"


int main(int argc, char* argv[])
{
	UDPClient client("127.0.0.1", 3333);

	const char* buf = "hello world";
	int len = strlen(buf);
	if (len == client.send_data(buf, len))
	{
		printf("send_data [%s] success\n", buf);
	}

	return 0;
}