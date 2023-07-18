#ifndef __server_input_h
#define __server_input_h

#include "socket/platform.h"
#include "command/servercmd.h"
#include <map>
#include <queue>
#include <iostream>

namespace rat{
    
    class ServerInput
    {
        public:
        static std::map< std::string, std::queue<ServerCmd>, std::less<> > server_request_map_;

        ServerInput() = default;
        void TakeUserInput() const;
    };

}
#endif