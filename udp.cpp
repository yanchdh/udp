#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[])
{

	/* int socket(int domain, int type, int protocol); */
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	printf("sockfd: %d\n", sockfd);
	if (sockfd == -1)
	{
		printf("socket failed: %d\n", errno);
		return -1;
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
	addr.sin_port = htons(3333);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(&(addr.sin_zero), 8);
	printf("addr: %d %d %d %d\n", sizeof(addr), addr.sin_family, addr.sin_port, addr.sin_addr.s_addr);

	/* int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen); */
	if (bind(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) < 0)
	{
		printf("bind failed: %d\n", errno);
		return -1;
	}

	return 0;
}