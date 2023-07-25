#ifndef RAT_SERVER_SERVER_H_
#define RAT_SERVER_SERVER_H_

#include "socket/platform.h"
#include "command/servercmd.h"
#include "server/handleclient.h"
#include "server/handleconnections.h"
#include "server/serverinput.h"

namespace rat{

    class Server
    {
    private:
        static std::string ip_address_;
        int port_ = -1;
        TcpSocket sock_;
        int max_client_ = 1;
    public:
        const static int kDefaultPort = 27015;
        Server() = default;
        explicit Server(int n_clients);
        void Clean();
        int CreateLocalServer();
        int CreateLocalServer(int port);
        int Listen();
        
        unsigned long long GetListenSocket() const;
        int GetMaxClient() const;
        int GetPort() const;
    };    
}

#endif // !__server_h
