#ifndef RAT_COMMAND_SEVERCMD_H_
#define RAT_COMMAND_SEVERCMD_H_

#include <string.h>

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

#include "command/command.h"

namespace rat{

    class ServerCmd: public Command
    {
    private:
        bool IsIPv4Address(const std::string& address) const;
        std::string ip_address_;
        int port_ = 0;
    public:
        ServerCmd();
        explicit ServerCmd(const std::string& input);
        std::string GetIpAddress() const;
        int GetPort() const;
        ~ServerCmd() = default;;
    };
    
}

#endif
