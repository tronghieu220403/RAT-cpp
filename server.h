#include "platform.h"
#include "servercmd.h"
#include <map>
#include <queue>
#include <iostream>
#include <mutex>
#include <thread>
namespace rat{

    class Server
    {
    private:
        static std::string ip_address_;
        int port_ = kDefaultPort;
        unsigned long long listen_socket_ = INVALID_SOCKET;
        int max_client_ = 1;
    public:
        static std::mutex mt;
        const static int kDefaultPort = 27015;
        Server() = default;
        void Clean();
        int CreateLocalServer();
        int Listen();
        void SetMaxClient(int n_clients);
        
        unsigned long long GetListenSocket();
        int GetMaxClient();
        ~Server(){
            if (listen_socket_ != INVALID_SOCKET){
                closesocket(listen_socket_);
            }
            Clean();
        };
    };
    
    class ServerInput
    {
        public:
        static std::map< std::string, std::queue<ServerCmd>, std::less<> > server_request_map_;

        ServerInput() = default;
        void TakeUserInput();

    };
    
    class HandleConnections
    {
        private:
        unsigned long long listen_socket_ = INVALID_SOCKET;
        int max_clients_ = 1;
        public:
        HandleConnections() = default;
        HandleConnections(unsigned long long listen_socket, int max_clients);
        void AcceptConnections();
    };

    class HandleClient
    {
        private:
        unsigned long long client_socket_ = INVALID_SOCKET;
        std::string ip;
	    std::string port;
	    std::string address;
        public:
        HandleClient(unsigned long long client_socket, sockaddr_in client_addr);
        ServerCmd WaitForRequest(std::string address);
        void ControlClient();
    };
}