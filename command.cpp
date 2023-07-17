#include "command.h"

namespace rat{

Command::Command() = default;;

Command::Command(const int type, const std::string_view& content){
    if (4 + content.size()){
        type_ = -1;
        return;
    }
    type_ = type;
    argument_ = content;
}

std::vector<char> Command::ToTcpPacket(){
    std::vector<char> v;
    if (type_ == -1){
        return v;
    }
    int tcp_size = 4 + 4 + static_cast<int>(strlen(&argument_[0])) + 1;
    v.resize(tcp_size);
    char *buffer = &*v.begin();

	memcpy(buffer, &tcp_size, 4);
	memcpy(buffer + 4, &type_, 4);
	memcpy(buffer + 8, &argument_[0], strlen(&argument_[0]));
    return v;
}

int Command::GetType() const{
    return type_;
}

std::string Command::GetArgument(){
    std::string s = argument_;
    return s;
}

void Command::SetType(int type) {
    type_ = type;
}

void Command::SetArgument(const std::string_view& argument) {
    argument_ = argument;
}


}