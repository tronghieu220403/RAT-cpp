#pragma once

#include "platform.h"
#include <vector>
namespace rat{

    class TcpSocket
    {
    private:
        unsigned __int64 socket_ = INVALID_SOCKET;   
        bool disconnected_ = false;
        void Close();
    public:
        TcpSocket();
        explicit TcpSocket(unsigned __int64 socket);
        bool Disconnected() const;
        
        int SafeRecv(char *buf, int len);
        int SafeSend(const char *buf, int len);
        
        long long RecvLongLong();
        int RecvInt();

        unsigned long long RecvUnsignedLongLong();
        unsigned int RecvUnsignedInt();
        
        int SendBytes(const char* buffer, int size);
        int SendBytes(std::vector<char> v);
        int SendInt(const int num);
        int SendLongLong(const long long num);
        
        void SetSocket(unsigned __int64 socket);
        unsigned long long GetSocket() const;
        
        ~TcpSocket(){
            Close();
        };
    };

}