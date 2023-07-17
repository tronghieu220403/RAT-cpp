#include "file.h"

namespace rat{

    File::File() = default;

    File::File(const std::string_view& file_path): file_path_(file_path){}

    void File::SetFilePath(std::string_view file_path)
    {
        file_path = file_path_;
    }

    int File::Remove()
    {
        return remove(&file_path_[0]);
    }

    int File::Remove(const std::string_view file_path)
    {
        return remove(&file_path[0]);
    }

    long long File::GetFileSize() const
    {
        if(!Exists()){
            return (long long)-1;
        }
        long long size = -1;
        try
        {
            std::filesystem::path p{ file_path_ };
            size = std::filesystem::file_size(p);
        }
        catch (std::filesystem::filesystem_error&)
        {
            size = (long long)-1;
        }
        catch (const std::exception&)
        {
            size = (long long)-1;
        }
        return size;
    }

    bool File::Append(const char *buffer, int len)
    {
        if(!Exists()){
            return false;
        }
        std::ofstream ofs_file(file_path_, std::ios_base::app | std::ifstream::binary);
        ofs_file.write(buffer, len);
        ofs_file.close();
        return true;
    }

    bool File::Exists() const
    {
        return std::filesystem::exists(file_path_);
    }

    bool File::Exists(const std::string_view file_path)
    {
        return std::filesystem::exists(file_path);
    }

    int File::SendFileThroughSocket(TcpSocket& sock)
    {
        if (!Exists()){
            return -1;
        }
        int status = 0;
        long long sent = 0;
        char buffer[1000];
        long long file_size = this->GetFileSize();
        if (file_size == (long long)-1) {
            sock.SendLongLong(file_size);
            return -1;
        }
        sock.SendLongLong(file_size);
        if (sock.Disconnected() == false)
        {
            return -2;
        }
        std::ifstream ifs_file(file_path_, std::ifstream::binary);

        while (true)
        {
            long long readSize = (((static_cast<long long>(1000)) < (file_size - sent)) ? (static_cast<long long>(1000)) : (file_size - sent));
            memset(buffer,0, sizeof(char) * 1000);
            ifs_file.read(buffer, readSize);
            sock.SafeSend(buffer, (int)readSize);
            if (sock.Disconnected())
            {
                status = -2;
            }
            else if (readSize < 1000) {
                status = 0;
                break;
            }
            sent += readSize;

        }
        ifs_file_.close();
        return status;
    }

    int File::RecvFileThroughSocket(TcpSocket& sock)
    {
        if(Exists()){
            Remove();
        }
        long long file_size = sock.RecvLongLong();
        if (file_size == -1)
        {
            return -1;
        }
        if (sock.Disconnected()) {
            return -1;
        }
        std::error_code err;
        std::filesystem::create_directories(std::filesystem::path(file_path_).parent_path().string(), err);
        std::ofstream ofs_file(file_path_, std::ios_base::app | std::ifstream::binary);
        long long bufferSize = (((static_cast<long long>(1000)) < (file_size)) ? (static_cast<long long>(1000)) : (file_size));
        auto iRecvResult = static_cast < long long>(0);
        auto received = static_cast <long long>(0);
	    auto* buffer = new char[1001];
        do {
            memset(buffer, 0, 1001);
            iRecvResult = sock.SafeRecv(buffer, static_cast<int>(bufferSize));
            if (iRecvResult > (long long)0)
            {
                Append(buffer, static_cast<int>(bufferSize));
                received += bufferSize;
                if (received == file_size) {
                    break;
                }
                bufferSize = (((static_cast<long long>(1000)) < (file_size - received)) ? (static_cast<long long>(1000)) : (file_size - received));
            }
            if (sock.Disconnected())
            {
                goto RETURN_FAIL;
            }
        } 
        while( iRecvResult > static_cast <long long>(0));
        
            ofs_file.close();
            delete[] buffer;
            return 0;
        RETURN_FAIL:
            ofs_file.close();
            Remove();
            delete[] buffer;
            return -1;

    }
}