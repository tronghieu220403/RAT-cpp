#include "socket/tcpsocket.h"

namespace rat{

    TcpSocket::TcpSocket() = default;

    TcpSocket::TcpSocket(const unsigned long long socket): socket_(socket){};

    bool TcpSocket::Disconnected() const
    {
        return disconnected_;
    }

    int TcpSocket::SafeRecv(char *buf, int len){
        int ori_len = len;
        int bytes_recv = 0;
        while(true){
            if (len == 0) 
            {
                bytes_recv = ori_len;
                break;
            }
            if (len != 0)
            {
                bytes_recv = recv(socket_, buf, len, 0);
                if (bytes_recv <= 0) {
                    Close();
                    break;
                }
                else {
                    buf = buf + bytes_recv;
                    len = len - bytes_recv;
                }
            }
        }
        return bytes_recv;
    }

    int TcpSocket::SafeSend(const char *buf, int len)
    {
        int ori_len = len;
        int bytes_send = 0;
        while(true){
            bytes_send = send(socket_, buf, len, 0);
            if (bytes_send < 0 || (bytes_send == 0 && len > 0))
            {
                Close();
                return -1;
            }
            else
            {
                buf = buf + bytes_send;
                len = len - bytes_send;
            }
            if (len == 0) {
                bytes_send = ori_len;
                break;
            }
        }
        return bytes_send;
    }


    long long TcpSocket::RecvLongLong()
    {
        char buffer[8];
        SafeRecv(buffer, 8);
        return *((long long*)buffer);
    }

    int TcpSocket::RecvInt()
    {
        char buffer[4];
        SafeRecv(buffer, 4);
        return *((int*)buffer);
    }

    unsigned int TcpSocket::RecvUnsignedInt()
    {
        char buffer[4];
        SafeRecv(buffer, 4);
        return *((unsigned int*)buffer);
    }

    unsigned long long TcpSocket::RecvUnsignedLongLong()
    {
        char buffer[8];
        SafeRecv(buffer, 8);
        return *((unsigned long long*)buffer);
    }

    std::vector<char> TcpSocket::RecvBytes(int size)
    {
    	std::vector<char> v;
    	v.resize(size);
        if (char *buffer = std::to_address(v.begin()); SafeRecv(buffer, size) != size)
        {
            v.clear();
            return v;
        }
        return v;
    }

    int TcpSocket::SendBytes(const char* buffer, int size)
    {
        return SafeSend(buffer, size);
    }

    int TcpSocket::SendBytes(std::vector<char> v)
    {
        return SafeSend(v.data(), static_cast<int>(v.size()));
    }

    int TcpSocket::SendInt(int num)
    {
        char c[4];
        memcpy(c, &num, 4);
        return SafeSend(c, 4);
    }

    int TcpSocket::SendLongLong(long long num)
    {
        char c[8];
        memcpy(c, &num, 8);
        return SafeSend(c, 8);
    }
    
    unsigned long long TcpSocket::GetSocket() const
    {
        return socket_;
    }

    void TcpSocket::Close()
    {
        if (disconnected_ == false)
        {
            #ifdef _WIN32
                closesocket(socket_);
            #elif __linux__
                close(socket_);
            #endif
        }
        disconnected_ = true;
        socket_ = INVALID_SOCKET;
    }
}