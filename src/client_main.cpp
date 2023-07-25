#include <iostream>

#include "client/client.h"

int main()
{
    std::string client_addr;
    int port;
    std::cout << "Server IP and port: " << std::endl;
    std::cin >> client_addr >> port;
    rat::Client client(client_addr, port);

    if (client.ConnectToServer() == 0)
    {
        std::cout << "Connected to " << client.GetServerAddress() << ", port " << client.GetServerPort() << std::endl;
        client.ReceiveCommand();
    }
    else{
        std::cout << "Can not connect to " << client.GetServerAddress() << ", port " << client.GetServerPort() << std::endl;
    }
}