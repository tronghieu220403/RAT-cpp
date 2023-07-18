#pragma once
#include "file.h"

namespace rat{

    File::File() = default;

    File::File(const std::string_view& file_path): file_path_(file_path){}

    void File::SetFilePath(std::string_view file_path)
    {
        file_path_ = file_path;
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

    bool File::Append(const char *buffer, int len) const
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
        if (sock.Disconnected())
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
                break;
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
        auto received = static_cast <long long>(0);
        std::ofstream ofs_file; 

        if (file_size == -1)
        {
            return -1;
        }
        if (sock.Disconnected()) {
            return -1;
        }
        std::error_code err;
        std::string file_name = file_path_.substr(file_path_.rfind('\\') + 1).substr(file_path_.rfind('/') + 1);
        std::string dir_path = file_path_.substr(0, file_path_.rfind('\\')).substr(0, file_path_.rfind('/'));
        if (file_name == dir_path)
        {
            dir_path = "";
        }
        if (dir_path.size() > 0)
        {
            std::filesystem::create_directories(dir_path, err);
            if (!std::filesystem::exists(dir_path))
            {
                global_mutex.lock();
                std::cout << "Fail to create folder for " + file_path_ << ".\nWrite file to current path\n" << std::flush;
                global_mutex.unlock();
                file_path_ = file_name;
            }
        }
        if (this->Exists())
        {
            global_mutex.lock();
            std::cout << "File is already exists. The system deleted this file for new file writing. \n" << std::flush;
            global_mutex.unlock();
            this->Remove();
        }
        ofs_file = std::ofstream(file_path_, std::ios_base::app | std::ifstream::binary);
        long long buffer_size = (((static_cast<long long>(1000)) < file_size) ? (static_cast<long long>(1000)) : file_size);
        do {
            if (std::vector<char> v = sock.RecvBytes(static_cast<int>(buffer_size)); v.size() > (long long)0)
            {
                Append(std::to_address(v.begin()), static_cast<int>(v.size()));
                received += buffer_size;
                buffer_size = (((static_cast<long long>(1000)) < (file_size - received)) ? (static_cast<long long>(1000)) : (file_size - received));
            }
            if (sock.Disconnected())
            {
                goto RETURN_FAIL;
            }
        } 
        while(received != file_size);
        
            ofs_file.close();
            return 0;
        RETURN_FAIL:
            ofs_file.close();
            this->Remove();
            return -1;

    }
}