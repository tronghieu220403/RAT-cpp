#ifndef RAT_SERVER_SERVERINPUT_H_
#define RAT_SERVER_SERVERINPUT_H_

#include <map>
#include <queue>
#include <iostream>

#include "socket/platform.h"
#include "command/servercmd.h"

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