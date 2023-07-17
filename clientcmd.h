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
        void HandleInput(const char* input);
    public:
        ClientCmd();
        ClientCmd(const TcpSocket& socket, const char* input);
        void SetSocket(unsigned long long socket);
        void SetSocket(const TcpSocket& socket);
        ~ClientCmd() = default;;
        bool execute();
    };
    
    
}