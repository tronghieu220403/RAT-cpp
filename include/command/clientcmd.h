#ifndef RAT_COMMAND_CLIENTCMD_H_
#define RAT_COMMAND_CLIENTCMD_H_

#include <string.h>

#include <iostream>
#include <vector>
#include <sstream>

#include "command/command.h"
#include "socket/tcpsocket.h"
#include "file/file.h"
#include "process/process.h"
#ifdef _WIN32
#include "registry/registry.h"
#endif

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

#endif // !__client_cmd_h
