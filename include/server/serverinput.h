#pragma once
#include "platform.h"
#include "servercmd.h"
#include <map>
#include <queue>
#include <iostream>
#include <thread>

namespace rat{
    
    class ServerInput
    {
        public:
        static std::map< std::string, std::queue<ServerCmd>, std::less<> > server_request_map_;

        ServerInput() = default;
        void TakeUserInput() const;
    };

}