#pragma once


#include <ws2tcpip.h>
#include <TlHelp32.h>

#include <string>
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