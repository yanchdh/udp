#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


class UDPServer
{
public:
	UDPServer(const char* ip, int port)
	{
		/* int socket(int domain, int type, int protocol); */
		int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		if (sockfd == -1)
		{
			printf("socket failed: %d\n", errno);
			return;
		}
		else
		{
			printf("socket success [%d]\n", sockfd);
		}

		/*
		struct sockaddr_in
		{
		    sa_family_t sin_family;
		    in_port_t sin_port;
		    struct in_addr sin_addr;
		    unsigned char sin_zero[8];
	  	};
	  	*/
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(ip);
		bzero(&(addr.sin_zero), 8);

		/* int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen); */
		if (bind(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) < 0)
		{
			printf("bind failed: %d\n", errno);
			return;
		}
		else
		{
			printf("bind success [%s:%d]\n", ip, port);
		}
	}
};


class UDPClient
{
	UDPClient(const char* ip, int port)
	{
		/* int socket(int domain, int type, int protocol); */
		int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		if (sockfd == -1)
		{
			printf("socket failed: %d\n", errno);
			return;
		}
		else
		{
			printf("socket success [%d]\n", sockfd);
		}

		/*
		struct sockaddr_in
		{
		    sa_family_t sin_family;
		    in_port_t sin_port;
		    struct in_addr sin_addr;
		    unsigned char sin_zero[8];
	  	};
	  	*/
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(ip);
		bzero(&(addr.sin_zero), 8);

		
	}
};


int main(int argc, char* argv[])
{
	UDPServer server("127.0.0.1", 3333);

	return 0;
}