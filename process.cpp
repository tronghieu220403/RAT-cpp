#include "process.h"
namespace rat
{

	Process::Process(): pid_(0) {};

	Process::Process(int id): pid_(id) {};

	Process::Process(const std::string_view& name): pid_(FindProcessByName(name)) {};

	int Process::GetPid() const{
		return pid_;
	}

	int Process::FindProcessByName(const std::string_view& name){
		int pid = 0;
		#ifdef _WIN32
			PROCESSENTRY32 entry;
			entry.dwSize = sizeof(PROCESSENTRY32);
			HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

			std::wstring wsTmp(name.begin(), name.end());

			if (Process32FirstW(snapshot, &entry) == TRUE)
			{
				while (Process32Next(snapshot, &entry) == TRUE)
				{
					if (wcscmp(entry.szExeFile, &wsTmp[0]) == 0)
					{
						pid = entry.th32ProcessID;
						break;
					}
				}
			}
			CloseHandle(snapshot);

		#elif __linux__
			DIR* dir = opendir("/proc");
			if (!dir) {
				return false;
			}

			bool found = false;
			dirent* entry;
			while ((entry = readdir(dir)) != nullptr && !found) {
				// check if the entry is a directory and its name is a number
				if (entry->d_type == DT_DIR && std::isdigit(entry->d_name[0])) {
					// read the process name from the cmdline file
					std::string cmdPath = std::string("/proc/") + entry->d_name + "/cmdline";
					//std::cout << cmdPath.c_str() << std::endl;
					
					std::ifstream cmdFile(cmdPath.c_str());
					std::string cmdLine;
					std::getline(cmdFile, cmdLine);

					// check if the process name matches
					if (!cmdLine.empty() && cmdLine.find(name) != std::string::npos) {
						pid = std::stoi(entry->d_name);
						break;
					}
					
				}
			}
			closedir(dir);

		#endif
			return pid;
	};

	void Process::SetPid(int pid){
		pid_ = pid;
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
			if (kill(pid_, SIGKILL)) {
        		return false;
			}
			return true;
		#endif
	}
}