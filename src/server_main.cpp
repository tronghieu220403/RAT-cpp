#pragma once
#include "server/server.h"
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

int main() {
    if (rat::Server server(1000); server.CreateLocalServer() == 0)
    {
        cout << "Server created on localhost, port " << server.GetPort() << endl << std::flush;
        if (server.Listen() != 0){
            cout << "Listen false" << endl << std::flush;
            return 0;
        }
        std::jthread connection_thread(&rat::HandleConnections::AcceptConnections, rat::HandleConnections(server.GetListenSocket(), server.GetMaxClient()));
        rat::ServerInput server_input;
        server_input.TakeUserInput();
    }
    return 0;
}