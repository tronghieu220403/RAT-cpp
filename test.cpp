#include "server.h"

#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

int main() {
    if (rat::Server server; server.CreateLocalServer() == 0)
    {
        server.SetMaxClient(1);
        cout << "Create oke" << endl;
        if (server.Listen() != 0) return 0;
        std::jthread connection_thread(&rat::HandleConnections::AcceptConnections, rat::HandleConnections(server.GetListenSocket(), server.GetMaxClient()));
        rat::ServerInput server_input;
        server_input.TakeUserInput();
    }
    return 0;
}