#pragma once

#if defined(_WIN32)
	#include <WS2tcpip.h>
	#include <TlHelp32.h>
#else
	#include <sys/wait.h>
	#include <dirent.h>
#endif
#include <string>
#include <string.h>
#include <fstream>

namespace rat
{
	class Process {
	private:
		int pid_;
	public:
		
		Process();
		explicit Process(int id);
		explicit Process(const std::string_view& name);
		
		int GetPid() const;
		void SetPid(int id);
		void SetPid(const std::string_view& name);
		bool KillSelf() const;

		static int FindProcessByName(const std::string_view& name);

	};
}