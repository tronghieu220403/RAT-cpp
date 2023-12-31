#ifndef RAT_FILE_FILE_H_
#define RAT_FILE_FILE_H_

#include <cstring>
#include <string>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <iostream>

#include "socket/tcpsocket.h"

namespace rat
{
	class File 
    {
	private:
        std::string file_path_;
        std::ifstream ifs_file_;
	public:
		File();
        explicit File(const std::string_view& file_path);
        
        void SetFilePath(const std::string_view file_path);
        
        bool Append(const char* buffer, int size) const;
        bool Exists() const;
        long long GetFileSize() const;
        int Remove();
        
        int SendFileThroughSocket(TcpSocket& sock);
        int RecvFileThroughSocket(TcpSocket& sock);

        static int Remove(const std::string_view file_path);
        static bool Exists(const std::string_view file_path);
	};

}

#endif