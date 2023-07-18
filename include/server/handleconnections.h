#ifndef __handle_connections_h
#define __handle_connections_h

#include "socket/platform.h"
#include "server/serverinput.h"
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

#endif