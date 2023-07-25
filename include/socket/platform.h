#ifndef RAT_SOCKET_PLATFORM_H_
#define RAT_SOCKET_PLATFORM_H_

#include <stdio.h>
#include <string.h>

#include <cstdint>
#include <string>
#include <mutex>

namespace rat {
	inline std::mutex global_mutex;
}

#if defined(_WIN32)

#pragma comment(lib, "Ws2_32.lib")

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif

#if !defined(_CRT_SECURE_NO_DEPRECATE)
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <WinSock2.h>
#include <WS2tcpip.h>

#define SOCKPP_SOCKET_T_DEFINED
using socket_t = SOCKET;

using socklen_t = int;
using in_port_t = uint16_t;
using in_addr_t = uint32_t;

using sa_family_t = u_short;

#define SHUT_RD SD_RECEIVE
#define SHUT_WR SD_SEND
#define SHUT_RDWR SD_BOTH

struct iovec
{
	void* iov_base;
	size_t iov_len;
};

#else
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#ifdef __FreeBSD__
#include <netinet/in.h>
#endif
#include <netdb.h>
#include <signal.h>

#include <cerrno>

#define INVALID_SOCKET static_cast<unsigned long long>(~0)
#define SOCKET_ERROR static_cast<int>(-1)

#endif

#endif