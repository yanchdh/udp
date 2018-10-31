/*
	udp.hpp
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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
  	bzero(&(addr.sin_zero), 8);
	addr.sin_addr.s_addr = inet_addr(ip);
	if (addr.sin_addr.s_addr == INADDR_NONE)
    {
		printf("[init_sockaddr_in] failed with incorrect ip[%s]\n", ip);
		return -1;
    }
    addr.sin_port = htons(port);
    addr.sin_family = family;
	return 0;
}

inline int make_socket_nonblocking(int fd)
{
	// int fcntl(int fd, int cmd, ... /* arg */ );
	int flags;
	if ((flags = fcntl(fd, F_GETFL, NULL)) < 0)
	{
		perror("fcntl F_GETFL fail");
		return -1;
	}
	if (!(flags & O_NONBLOCK))
	{
		if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		{
			perror("fcntl F_SETFL fail");
			return -1;
		}
	}
	return 0;
}

inline ssize_t _sendto(int fd, const struct sockaddr* addr, const void* buf, int len, int flags=0)
{
	/*
	ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
                  const struct sockaddr *dest_addr, socklen_t addrlen);
	*/
	socklen_t addrlen = sizeof(struct sockaddr);
	ssize_t n = sendto(fd, buf, len, flags, addr, addrlen);
	if (n < 0)
	{
		perror("sendto fail");
		return n;
	}
	return n;
}

inline ssize_t _recvfrom(int fd, struct sockaddr* addr, void* buf, int len, int flags=0)
{
	/*
	ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                struct sockaddr *src_addr, socklen_t *addrlen);
	*/
	socklen_t addrlen = sizeof(struct sockaddr);
	ssize_t n = recvfrom(fd, buf, len, flags, addr, &addrlen);
	if (n < 0)
	{
		if (errno == EAGAIN)
		{
			return 0;
		}
		perror("recvfrom fail");
	}
	return n;
}

class UDPServer
{
public:
	UDPServer(const char* ip, int port)
	{
		/* int socket(int domain, int type, int protocol); */
		m_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (m_sockfd == -1)
		{
			perror("socket fail");
			exit(EXIT_FAILURE);
		}

		if (make_socket_nonblocking(m_sockfd) < 0)
		{
			perror("make_socket_nonblocking fail");
			close(m_sockfd);
			exit(EXIT_FAILURE);
		}

		struct sockaddr_in addr;
		if (init_sockaddr_in(AF_INET, ip, port, addr))
		{
			perror("init_sockaddr_in fail");
			close(m_sockfd);
			exit(EXIT_FAILURE);
		}

		/* int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen); */
		if (bind(m_sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) < 0)
		{
			perror("bind fail");
			close(m_sockfd);
			exit(EXIT_FAILURE);
		}
		printf("UDPServer[%s:%d]\n", ip, port);
	}

	void loop()
	{
		char buf[1024];
	    struct sockaddr_in addr;
		while (1)
		{
			int n = _recvfrom(m_sockfd, (struct sockaddr*)&addr, buf, sizeof(buf) - 1, MSG_DONTWAIT);
			if (n > 0)
			{
				buf[n] = 0;
				printf("%s:%d say: [%s]\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), buf);
				//_sendto(m_sockfd, (struct sockaddr*)&addr, buf, n);
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
		m_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (m_sockfd == -1)
		{
			perror("socket");
			exit(EXIT_FAILURE);
		}

		if (make_socket_nonblocking(m_sockfd) < 0)
		{
			perror("make_socket_nonblocking fail");
			close(m_sockfd);
			exit(EXIT_FAILURE);
		}

		if (init_sockaddr_in(AF_INET, ip, port, m_addr))
		{
			perror("init_sockaddr_in fail");
			close(m_sockfd);
			exit(EXIT_FAILURE);
		}
		printf("UDPClient[%s:%d]\n", ip, port);
	}

	void loop()
	{
		int num = 0;
		char buf[1024];
		while (1)
		{
			usleep(10000);
			num += 1;
			int n = snprintf(buf, sizeof(buf), "%d", num);
			_sendto(m_sockfd, (struct sockaddr*)&m_addr, buf, n);
			printf("sendto %s:%d: [%s]\n", inet_ntoa(m_addr.sin_addr), ntohs(m_addr.sin_port), buf);
		}
	}

private:
	int m_sockfd;
	struct sockaddr_in m_addr;
};