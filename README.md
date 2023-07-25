# Single server - multi clients TCP communication via IPv4 

This is a small single server - multi clients TCP communication via IPv4, works on both 64-bit Windows and Linux OS.

- [Problem](#problem)
- [Problem Solving Proposal](#problem-solving-proposal)
- [Features](#features)
- [Commands](#commands)
- [Folder structure](#folder-structure)
- [Requirements](#requirements)
- [Coding style](#coding-style)

Problem
-------

Write a RAT software where the server will command its clients to perform the following tasks: 
- Get a file.
- Kill a process.
- Delete a registry.

Advanced requirements: 
- The server can issue commands to multiple clients at the same time.
- The server can get large files.

Problem Solving Proposal
-------------------------
The following are some things to be aware of, along with my solutions:

* Manage multi clients at the same time: Using multi-thread, each client has a queue to stored commands, and every client will have a function to await server commands.

* Get a file with large size: We are **unable to load files to RAM when dealing with huge files**. Therefore, the approach is to find functions that can **read or write data without putting it into RAM**.

* Transfer file over TCP: 
	* We will **split the files into smaller pieces of data** and then send them in order. 
	* The server's data receiving speed may be faster than the client's sending speed, so each time we receive data, we will wait until we have received the desired number of bytes.
	* The client will tell the server about the file's size. If the size if `-1`, stop the operation. Otherwise, start sending data.

Features
---
* Remote Command Execution via:
  * CMD

Commands
---
|Command|Syntax|Comment|
|-------|------|---------|
|kill pid|`[client IPv4] [client port] kill pid [pid]]`|Kill a process by PID|
|kill pname|`[client IPv4] [client port] kill pname [name]`|Kill a process by name|
|delete registry|`[client IPv4] [client port] delete registry [root key] [sub key]`|Delete a registry|
|get file|`[client IPv4] [client port] get file [file path]`|Get a file from client|
|exit|`exit`|Turn off the server|

Folder structure
----------------
```
.
├── src              
│   └── client_main.cpp                                        # code for client
│   └── server_main.cpp                                        # code for server
│   │
├── inlucde
│   └── client                                            # dependencies for client
│   │   └── client.h
│   │   └── client.cpp
│   └── server                                            # dependencies for server 
│   │   └── server.h
│   │   └── server.cpp
│   │   └── handleclient.h
│   │   └── handleclient.cpp
│   │   └── handleconnections.h
│   │   └── handleconnections.cpp
│   │   └── serverinput.h
│   │   └── serverinput.cpp
│   └── socket                                            # dependencies for TCP Socket
│   │   └── platform.h
│   │   └── tcpsocket.h
│   │   └── tcpsocket.cpp
│   └── command                                           # communication rules between server and clients 
│   │   └── command.h
│   │   └── command.cpp
│   │   └── clientcmd.h
│   │   └── clientcmd.cpp
│   │   └── servercmd.h
│   │   └── servercmd.cpp
│   └── file                                              # dependencies for file IO
│   │   └── file.h
│   │   └── file.cpp
│   └── process                                           # dependencies for processes management
│   │   └── process.h
│   │   └── process.cpp
│   └── registry                                          # dependencies for Windows Registry
│   │   └── registry.h
│   │   └── registry.cpp
│   │
├── release              
│   └── x64
│   │   └── Linux                               # executable file for server and client on Linux
│   │   │   └── client_main		
│   │   │   └── server_main			
│   │   └── Windows                             # executable file for server and client on Windows
│   │   │   └── client_main.exe			
│   │   │   └── server_main.exe
│   │
────────────	
```


Requirements
---
* C++ 20
* Supported Operating Systems (64-bit)
  * Windows
  * Linux

Coding style
------------
[Google C++ Style](https://google.github.io/styleguide/cppguide.html)
