#pragma once

#include "platform.h"
#include <vector>
namespace rat{

    class TcpSocket
    {
    private:
        unsigned long long socket_ = INVALID_SOCKET;   
        bool disconnected_ = false;
        void Close();
    public:
        TcpSocket();
        explicit TcpSocket(unsigned long long socket);
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
        
        void SetSocket(unsigned long long socket);
        unsigned long long GetSocket() const;
        
        ~TcpSocket(){
            Close();
        };
    };

}