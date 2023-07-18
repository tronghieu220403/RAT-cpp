#include "platform.h"
#include "tcpsocket.h"
#include "clientcmd.h"

namespace rat{

    class Client
    {
    private:
        std::string ip_address_;
        int port_;
        TcpSocket sock_;
    public:
        const static int DEFAULT_PORT_ = 27015;
        Client() = default;
        Client(const std::string_view ip_address, int port);
        long long ConnectToServer(const std::string_view, int port);
        void ReceiveCommand();
        void Clean();
        ~Client() = default;;
    };
    
}