# Single server - multi clients TCP communication via IPv4 

This is a small single server - multi clients TCP communication via IPv4 on Windows OS 64-bit.

- [Problem](#problem)
- [Abstract](#abstract)
- [Features](#features)
- [Commands](#commands)
- [Folder structure](#folder-structure)
- [Requirements](#requirements)

Problem
-------

Write RAT software: the server commands the client to perform tasks: 
- Get a file.
- Kill a process.
- Delete a registry.

Advanced requirements: 
- The server can issue commands to multiple clients at the same time.
- The server can get large files.

Abstract
--------
The following are some things to be aware of, along with my solutions:

* Manage multi clients at the same time: Using multi-thread, commands for each client will be stored in a queue, and every client will have a function to await server commands.

* Get a file with large size: We are **unable to load files to RAM when dealing with huge files**. Therefore, the approach is to find functions that can **read or write data without putting it into RAM**.

* Transfer file over TCP: 
	* We will **split the files into smaller pieces of data** and then send them in order. 
	* The server's data receiving speed may be faster than the client's sending speed, so each time we receive data, we will wait until we have received the desired number of bytes.
	* The client will tell the server about the file's size. If the size if `-1` or the server can not create file, stop the operation. Otherwise, start sending data.

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
├── server
│   └── server.cpp				# code for server
│   └── x64
│   │   └── Release
│   │   │   └── server.exe			# executable file for server
├── client              
│   └── client.cpp				# code for client
│   └── x64
│   │   └── Release
│   │   │   └── client.exe			# executable file for client
```


Requirements
---
* C++ 20
* Supported Operating Systems (64-bit)
  * Windows OS
  * Linux

