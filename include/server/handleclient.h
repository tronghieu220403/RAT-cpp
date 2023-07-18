#ifndef __handle_client_h
#define __handle_client_h

#include "socket/tcpsocket.h"
#include "command/servercmd.h"
#include "server/serverinput.h"
#include "file/file.h"
#include <thread>

namespace rat{

    class HandleClient
    {
        private:
        TcpSocket sock;
        std::string ip;
	    std::string port;
	    std::string address;
        public:
        HandleClient(unsigned long long client_socket, sockaddr_in client_addr);
        ServerCmd WaitForRequest(std::string address) const;
        void ControlClient();
    };
}

#endif