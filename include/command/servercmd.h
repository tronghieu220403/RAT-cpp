#pragma once
#include "command/command.h"
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

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