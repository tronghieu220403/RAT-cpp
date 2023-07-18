#pragma once
#include "platform.h"
#include "servercmd.h"
#include <map>
#include <queue>
#include <iostream>
#include <thread>

namespace rat{
    
    class HandleConnections
    {
        private:
        unsigned long long listen_socket_ = INVALID_SOCKET;
        int max_clients_ = 1;
        public:
        HandleConnections() = default;
        HandleConnections(unsigned long long listen_socket, int max_clients);
        void AcceptConnections() const;
    };

}