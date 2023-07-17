#include <dirent.h>
#include <string>
#include <iostream>
#include <fstream>

bool isProcessRunning(const std::string& processName) {
    DIR* dir = opendir("/proc");
    if (!dir) {
        std::cerr << "Error opening /proc directory" << std::endl;
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
            if (!cmdLine.empty() && cmdLine.find(processName) != std::string::npos) {
                found = true;
                std::cout << entry->d_name << std::endl;
                break;
            }
            
        }
    }
    closedir(dir);
    return found;
}

int main(){
    std::cout << isProcessRunning("github-desktop") << std::endl;
}
