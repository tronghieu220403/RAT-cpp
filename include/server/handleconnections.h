#ifndef RAT_SERVER_HANDLECONNECTIONS_H_
#define RAT_SERVER_HANDLECONNECTIONS_H_

#include <thread>

#include "socket/platform.h"
#include "server/serverinput.h"

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