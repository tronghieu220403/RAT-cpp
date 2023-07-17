#include "platform.h"
#include "tcpsocket.h"
namespace rat{

    class Client
    {
    private:
        
    protected:
        std::string ip_address_;
        int port_;
        TcpSocket sock;
    public:
        const static int DEFAULT_PORT_ = 17015;
        Client(std::string ip_address, int port);
        Client(std::string input);
        void ConnectToServer(std::string server_addr, int port);
        void ReceiveOrder(std::string command, TcpSocket &sock);
        void Clean();
        ~Client(){};
    };
    
}