#pragma once
#include "server/handleconnections.h"

namespace rat{

HandleConnections::HandleConnections(unsigned long long listen_socket, int max_clients) : listen_socket_(listen_socket), max_clients_(max_clients){};

void HandleConnections::AcceptConnections() const
{
    if (listen_socket_ == 0)
    {
        return;
    }
	while (true) {
        if (ServerInput::server_request_map_.size() >= max_clients_)
        {
            continue;
        }
		sockaddr_in client_addr{};
		#ifdef _WIN32
			int addrLen = sizeof(client_addr);
			unsigned long long client_socket = accept(listen_socket_, reinterpret_cast<sockaddr*>(&client_addr), &addrLen);
		#elif __linux__
			socklen_t addrLen = static_cast<socklen_t>(sizeof(client_addr));
			unsigned long long client_socket = accept(listen_socket_, reinterpret_cast<sockaddr*>(&client_addr), &addrLen);
		#endif
		if (client_socket != INVALID_SOCKET) {
			std::jthread backgroundThread(&HandleClient::ControlClient, HandleClient(client_socket, client_addr));
		}
	}

}

}