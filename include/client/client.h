#pragma once

#include "clientcmd.h"

namespace rat{

    class Client
    {
    private:
        std::string server_ip_address_;
        int server_port_;
        TcpSocket sock_;
    public:
        const static int kDefaultPort = 27015;
        Client() = default;
        Client(const std::string_view server_ip_address, int server_port);
        long long ConnectToServer();
        void ReceiveCommand();
        void Clean();
        int GetServerPort() const;
        std::string GetServerAddress() const;
        ~Client() = default;;
    };
    
}