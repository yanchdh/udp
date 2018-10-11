/*
	udp.hpp
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


inline int init_sockaddr_in(sa_family_t family, const char* ip, in_port_t port, struct sockaddr_in& addr)
{
	/*
	struct sockaddr_in
	{
	    sa_family_t sin_family;
	    in_port_t sin_port;
	    struct in_addr sin_addr;
	    unsigned char sin_zero[8];
  	};
  	*/
	addr.sin_addr.s_addr = inet_addr(ip);
	if (addr.sin_addr.s_addr == INADDR_NONE)
    {
		printf("[init_sockaddr_in] failed with incorrect ip[%s]\n", ip);
		return 1;
    }
    addr.sin_family = family;
	addr.sin_port = htons(port);
	bzero(&(addr.sin_zero), 8);
	return 0;
}


class UDPServer
{
public:
	UDPServer(const char* ip, int port)
	{
		/* int socket(int domain, int type, int protocol); */
		m_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		if (m_sockfd == -1)
		{
			perror("socket");
			return;
		}
		printf("socket success [%d]\n", m_sockfd);

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
		if (init_sockaddr_in(AF_INET, ip, port, addr))
		{
			close(m_sockfd);
			return;
		}

		/* int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen); */
		if (bind(m_sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) < 0)
		{
			close(m_sockfd);
			perror("bind");
			return;
		}
		printf("bind success [%s:%d]\n", ip, port);
	}

	void loop()
	{
		char buff[1024];
	    struct sockaddr_in clientAddr;
	    socklen_t clientAddrLen = sizeof(clientAddr);
		while (1)
		{
			/*
			ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                        struct sockaddr *src_addr, socklen_t *addrlen);
			*/
			int n = recvfrom(m_sockfd, buff, 1023, 0, (struct sockaddr*)&clientAddr, &clientAddrLen);
			if (n > 0)
			{
				buff[n] = 0;
				printf("%s:%d say: %s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buff);
			}
			else
			{
				perror("recvfrom");
			}
		}
	}

private:
	int m_sockfd;
};


class UDPClient
{
public:
	UDPClient(const char* ip, int port)
	{
		/* int socket(int domain, int type, int protocol); */
		m_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		if (m_sockfd == -1)
		{
			perror("socket");
			return;
		}
		printf("socket success [%d]\n", m_sockfd);

		if (init_sockaddr_in(AF_INET, ip, port, m_addr))
		{
			close(m_sockfd);
			return;
		}
	}

	int send_data(const char* buf, int len)
	{
		/*
		ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
                      const struct sockaddr *dest_addr, socklen_t addrlen);
		*/
		ssize_t sz = sendto(m_sockfd, buf, len, 0, (struct sockaddr *)&m_addr, sizeof(struct sockaddr));
		if (-1 == sz)
		{
			perror("sendto");
		}
		return sz;
	}

private:
	int m_sockfd;
	struct sockaddr_in m_addr;

};