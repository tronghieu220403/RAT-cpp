#if defined(_WIN32)
#include <ws2tcpip.h>
#include <TlHelp32.h>
#else

#endif
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

	Process::Process(): pid_(0) {};

	Process::Process(int id): pid_(id) {};

	Process::Process(const std::string_view& name): pid_(FindProcessByName(name)) {};

	int Process::GetPid() const{
		return pid_;
	}

	int Process::FindProcessByName(const std::string_view& name){
		int id = 0;
		#ifdef _WIN32
			PROCESSENTRY32 entry;
			entry.dwSize = sizeof(PROCESSENTRY32);
			HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

			std::wstring wsTmp(name.begin(), name.end());

			if (Process32First(snapshot, &entry) == TRUE)
			{
				while (Process32Next(snapshot, &entry) == TRUE)
				{
					if (wcscmp(entry.szExeFile, &wsTmp[0]) == 0)
					{
						id = entry.th32ProcessID;
						break;
					}
				}
			}
			CloseHandle(snapshot);

		#elif __linux__

		#endif
			return id;
	};

	void Process::SetPid(int id){
		pid_ = id;
	};

	void Process::SetPid(const std::string_view& name){
		pid_ = FindProcessByName(name);
	};

	bool Process::KillSelf() const{
		if (!pid_)
		{
			return false;
		}
		#ifdef _WIN32
			if (HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid_); hProcess != nullptr)
			{
				TerminateProcess(hProcess, 0);
				CloseHandle(hProcess);
				return true;
			}
			return false;
		#elif __linux__

		#endif
	}
}