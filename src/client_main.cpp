// test_client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "client/client.h"

#include <iostream>

int main()
{
    std::string client_addr;
    int port;
    std::cout << "Server IP and port: " << std::endl;
    std::cin >> client_addr >> port;
    rat::Client client("client_addr", port);
    //rat::Client client("192.168.1.5", 27015);

    if (client.ConnectToServer() == 0)
    {
        std::cout << "Connected to " << client.GetServerAddress() << ", port " << client.GetServerPort() << std::endl;
        client.ReceiveCommand();
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
