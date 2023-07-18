#pragma once
#include "platform.h"
#include "servercmd.h"
#include <map>
#include <queue>
#include <iostream>
#include <thread>

namespace rat{

    class HandleClient
    {
        private:
        unsigned long long client_socket_ = INVALID_SOCKET;
        std::string ip;
	    std::string port;
	    std::string address;
        public:
        HandleClient(unsigned long long client_socket, sockaddr_in client_addr);
        ServerCmd WaitForRequest(std::string address) const;
        void ControlClient();
    };
}