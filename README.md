![хаха инет хреновый](img/p2ptransfer.jpg)
# Resume
This program is **under development** and doesn't present the whole range of possibilities.
At this point, **p2pTransfer** application allows us to:
* Application is ported both for Linux and Windows.
* Send/Receive some file within one LAN.
To do so, clone the repo and build it for your system (building script is for linux only).

For Linux:
```bash
git clone https://github.com/LeeDoor/p2ptransfer --depth 1
./build.sh
# On your receiving machine, run:
./run_server.sh <PORT>
# On your sending machine, run:
./run_client.sh <ADDRESS> <PORT> <MESSAGE>
```
For Windows:
```bash
git clone https://github.com/LeeDoor/p2ptransfer --depth 1
./windows_build.sh
# Copy the result executables to your windows machine.
# They are in win_build/client and win_build/server directories.

# Open PowerShell and run the script.
# On your receiving machine, run:
./p2plisten.exe <PORT>
# On your sending machine, run:
./p2ptransfer.exe <ADDRESS> <PORT> <FILENAME>
```
Let's dive into CLI parameters:
1. `run_server.sh` or `p2plisten.exe`:
    * **`<PORT>`** - the port, where you are listening to connections.
2. `run_client.sh` or `p2ptransfer.exe`:
    * **`<ADDRESS>`** - IPv4 address of receiving machine. Get it using `ifconfig` or `ipconfig` commands.
    * **`<PORT>`** - Port of receiving machine you entered in previous script.
    * **`<FILENAME>`** - the path to file you want to send.

After sending, on your listening machine there will be the file called `READED_<FILENAME>`.
# Introduction
**P2PTransfer** is an application to transfer any files using P2P connection. You can connect one PC to another and send any files directly. If you use local Wi-Fi or even the Ethernet cable, you can speed up file transfer many times.
# The Server (file acceptor)
The server is the part of the project, which takes responsibility about receiving file. To reach an agreement about file transferation the server and the client would use the [language](#The language).
# The language
When a client looks for opened servers, it connects to everyone in a row and sends the **send request**:
```
REQUEST 
FILE bigfile.txt
SIZE 25000
```
* REQUEST means *I want to connect to you*.
* FILE means *I want to send you a file <filename>*.
* SIZE means *The file i want to send to you has a size <bytes> bytes*.

When a server gets a send request it sends a **send permission**. It consists of the `PERMISSION` word and sending filename:
```
PERMISSION 
FILE bigfile.txt
```
After the server sent a permission, it starts listening for a file. It gathers *<bytes>* amount of data and then replies with the **report**:
1. If transfer was successful, then the report is:
```
TRANSFERED 
FILE bigfile.txt
```
2. If transfer failed, the report is:
```
FAILED 
FILE bigfile.txt
```
