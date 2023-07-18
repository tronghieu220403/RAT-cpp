#include "server/serverinput.h"

namespace rat{

inline std::map< std::string, std::queue<ServerCmd>, std::less<> > ServerInput::server_request_map_;

void ServerInput::TakeUserInput() const
{
	while(true){
		std::string command;
		std::getline(std::cin, command);
		if (command.empty()) continue;
		if (command.back() == '\n')
		{
			command.pop_back();
		}
		if (command.size() > (size_t)900){
            global_mutex.lock();
			std::cout << "The command is too long, please try agian." << std::endl << std::flush;
            global_mutex.unlock();
		}
        if (command.size() >= 4 && command.starts_with("exit"))
        {
			return;
        }
        ServerCmd cmd(command);
        if (cmd.GetType() == -1)
        {
            continue;
        } 

		std::string address = cmd.GetIpAddress() + ":" + std::to_string(cmd.GetPort());
        if (ServerInput::server_request_map_.contains(address))
        {
            ServerInput::server_request_map_[address].push(cmd);
        }
    }
	return;
}

}
