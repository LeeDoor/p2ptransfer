# Resume
Current state of the program allows:
* Send a file to some IP.
* Get a file from some IP.
# Introduction
**P2PTransfer** is an application to transfer any files using P2P connection. You can connect one PC to another and send any files directly. If you use local Wi-Fi or even the Ethernet cable, you can speed up file transfer many times.
# The Server (file acceptor)
The server is the part of the project, which takes responsibility about receiving file. To reach an agreement about file transferation the server and the client would use the [language](#The language).
# The language
When a client looks for opened servers, it connects to everyone in a row and sends the **send request**:
```
REQUEST bigfile.txt
SIZE 25000
```
* REQUEST means *I want to connect to you and send some file <filename>*.
* SIZE means *The file i want to send to you has a size <bytes> bytes*.

When a server gets a send request it sends a **send permission**. It consists of the `PERMISSION` word and sending filename:
```
PERMISSION bigfile.txt
```
After the server sent a permission, it starts listening for a file. It gathers *<bytes>* amount of data and then replies with the **report**:
1. If transfer was successful, then the report is:
```
TRANSFERED bigfile.txt
```
2. If transfer failed, the report is:
```
FAILED bigfile.txt
```
