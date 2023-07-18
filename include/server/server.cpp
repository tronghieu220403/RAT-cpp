#include "server/server.h"

namespace rat{

Server::Server(int n_clients) : max_client_(n_clients) {};

int Server::CreateLocalServer()
{
	return CreateLocalServer(kDefaultPort);
}

int Server::CreateLocalServer(int port)
{
    int i_result;
	struct addrinfo* result = nullptr;
	unsigned long long listen_socket;

	// Initialize Winsock
	#ifdef _WIN32
	WSADATA wsaData;
	i_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (i_result != 0) {
		return i_result;
	}
	#endif

	struct addrinfo hints;
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	i_result = getaddrinfo(nullptr, &(std::to_string(port))[0], &hints, &result);
	if (i_result != 0) {
		goto CREATE_FAILED;
	}

	// Create a SOCKET for the server to listen for client connections.
	listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listen_socket == INVALID_SOCKET) {
		freeaddrinfo(result);
		goto CREATE_FAILED;
	}
	
	port_ = port;
	// Setup the TCP listening socket
	i_result = bind(listen_socket, result->ai_addr, (int)result->ai_addrlen);
	#ifdef _WIN32
		if (i_result == SOCKET_ERROR)
	#elif __linux
		if (i_result < 0)
	#endif
		{
			freeaddrinfo(result);
			#ifdef _WIN32
				closesocket(listen_socket);
			#elif __linux__
				close(listen_socket);
			#endif
			goto CREATE_FAILED;
		}

	sock_ = TcpSocket(listen_socket);
	freeaddrinfo(result);
    return 0;

	CREATE_FAILED:
		#ifdef _WIN32
			WSACleanup();
			return WSAGetLastError();
		#elif __linux__
			return i_result;
		port_ = -1;
		#endif

}

int Server::Listen()
{
	if (sock_.GetSocket() == INVALID_SOCKET) return -1;
	#ifdef _WIN32
		if (listen(sock_.GetSocket(), 0x7fffffff) == SOCKET_ERROR) {
			std::cout << "Failed to listen on socket. errno: " << WSAGetLastError() << std::endl << std::flush;
			WSACleanup();
			Clean();
			return -1;
		}
	#else
    	if (listen(sock_.GetSocket(), GetMaxClient()) < 0) {
    		std::cout << "Failed to listen on socket. errno: " << errno << std::endl << std::flush;
			Clean();
			return -1;
		}

	#endif
	return 0;	

}

unsigned long long Server::GetListenSocket() const
{
	return sock_.GetSocket();
}

int Server::GetPort() const
{
	return port_;
}

int Server::GetMaxClient() const
{
	return max_client_;
}

void Server::Clean()
{
	if (sock_.GetSocket() != INVALID_SOCKET) {
		sock_.Close();
	}
	#ifdef _WIN32
		WSACleanup();
	#endif
}

}
