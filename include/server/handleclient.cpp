#pragma once
#include "server/handleclient.h"

namespace rat{

HandleClient::HandleClient(unsigned long long client_socket, sockaddr_in client_addr): client_socket_(client_socket)
{
	sock = TcpSocket(client_socket_);
	std::string ip_addr;
	ip_addr.resize((long long)20);
	ip = inet_ntop(AF_INET, &client_addr.sin_addr, &ip_addr[0], 20);
	port = std::to_string(htons(client_addr.sin_port));
	address = ip + ":" + port;
	global_mutex.lock();
	std::cout << "Client connected: " << address << std::endl << std::flush;
    ServerInput::server_request_map_[address] = std::queue<ServerCmd>();
	global_mutex.unlock();
}

void HandleClient::ControlClient()
{
	if (sock.GetSocket() == INVALID_SOCKET)
	{
		return;
	}

	while(true){
		sock.SafeSend("", 0);
		if (sock.Disconnected())
		{
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
        ServerCmd server_cmd = WaitForRequest(address);
		if (server_cmd.GetType() == -1) {
			continue;
		}
		sock.SendBytes(server_cmd.ToTcpPacket());
		if (server_cmd.GetType() == static_cast<int>(rat::Command::CommandType::kClientSendFile))
		{
			std::string file_path = server_cmd.GetArgument();
        	std::replace(file_path.begin(), file_path.end(), '\\', '/');
			std::string file_name = file_path.substr(file_path.rfind('/') + 1);

			File f(file_name);
			f.RecvFileThroughSocket(sock);
		}
	}

	global_mutex.lock();
    ServerInput::server_request_map_.erase(address);
	std::cout << "Client disconnected: " << address << std::endl << std::flush;
	global_mutex.unlock();
	return;

}


ServerCmd HandleClient::WaitForRequest(std::string address) const
{
	global_mutex.lock();
	auto it = ServerInput::server_request_map_.find(address);
	if (it == ServerInput::server_request_map_.end()) {
		global_mutex.unlock();
		return ServerCmd();
	}
	std::queue<ServerCmd>& my_queue = it->second;
	long long queue_sz = my_queue.size();
	global_mutex.unlock();

    // If client has request from the server.
	if (queue_sz > 0) {
		global_mutex.lock();
		ServerCmd s = my_queue.front();
		my_queue.pop();
		global_mutex.unlock();
		return s;
	}
	return ServerCmd();

}

}