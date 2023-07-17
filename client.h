#include "platform.h"
#include "tcpsocket.h"
namespace rat{

    class Client
    {
    private:
        std::string ip_address_;
        int port_;
        TcpSocket sock;
    public:
        const static int DEFAULT_PORT_ = 17015;
        Client() = default;
        Client(const std::string_view ip_address, int port);
        explicit Client(std::string input);
        long long ConnectToServer(const std::string_view, int port);
        void ReceiveOrder(std::string command, TcpSocket &sock);
        void Clean();
        ~Client() = default;;
    };
    
}