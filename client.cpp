#include "client.h"

rat::Client::Client(const std::string_view ip_address, int port)
{
}

rat::Client::Client(std::string input)
{
}

long long rat::Client::ConnectToServer(const std::string_view server_addr, int port)
{
	int i_result;
	
	auto connect_socket = INVALID_SOCKET;
	struct sockaddr_in client_service;

	#ifdef _WIN32
		WSADATA wsa_data;
		//----------------------
		// Initialize Winsock
		i_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
		if (i_result != NO_ERROR) {
			return i_result;
		}
	#endif
	//----------------------
	// Create a SOCKET for connecting to server
	connect_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect_socket == INVALID_SOCKET) {
		goto CONNECT_FAILED;
	}

	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	client_service.sin_family = AF_INET;
	client_service.sin_port = htons(static_cast<u_short>(port));
	#ifdef _WIN32
		InetPtonA(AF_INET, &server_addr[0], &client_service.sin_addr.s_addr);
	#elif __linux__
		inet_pton(AF_INET, &server_addr[0], &client_service.sin_addr);
	#endif
	//----------------------
	// Connect to server.
	i_result = connect(connect_socket, (sockaddr*)&client_service, sizeof(client_service));
	if (i_result == SOCKET_ERROR) {
		goto CONNECT_FAILED;
	}
	sock = TcpSocket(connect_socket);
	return connect_socket;

	CONNECT_FAILED:
		#ifdef _WIN32
			if (connect_socket != INVALID_SOCKET) {
				closesocket(connect_socket);
			}
			WSACleanup();
			return WSAGetLastError();
		#elif __linux__
			if (connect_socket != INVALID_SOCKET) {
				close(connect_socket);
			}
			return i_result;
		#endif //
}

void rat::Client::ReceiveOrder(std::string command, TcpSocket& sock)
{
}

void rat::Client::Clean()
{
}