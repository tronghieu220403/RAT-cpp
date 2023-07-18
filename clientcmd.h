#pragma once

#include "command.h"
#include "tcpsocket.h"
#include "file.h"
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