#ifndef __tcp_socket_h
#define __tcp_socket_h

#include "socket/platform.h"
#include <vector>
namespace rat{

    class TcpSocket
    {
    private:
        unsigned long long socket_ = INVALID_SOCKET;   
        bool disconnected_ = false;
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
        std::vector<char> RecvBytes(int size);     

        int SendBytes(const char* buffer, int size);
        int SendBytes(std::vector<char> v);
        int SendInt(const int num);
        int SendLongLong(const long long num);
        
        unsigned long long GetSocket() const;
        void Close();
    };

}

#endif