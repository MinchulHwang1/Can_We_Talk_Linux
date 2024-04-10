# CanWeTalkSystem

## Description
This application bundle is a combination of client and server developed to communicate over a network using TCP/IP sockets in C language.
## Chat-client
The client application was designed using the ncurses library comprised of mutiple windows to enhance usuability to communicate seamlessly. For this project, we named it ChatLite
due to its simplicity.
### How to use:
1. The client application is built using the following command:
   ```bash
   make all
   ```
   Ensure that this command is run in the `bin` directory of the client application.
2. Once the client application binary is built you can run it using the command:
   ```bash
   ./chat-client
   ```
3. The chat-client application will have to be ran using command line arguments as follows:
4. ```bash
   ./chat-client -user<USERNAME> -server<SEVERIP>
   ```
   `NOTE`: The client can also resolve IP by hostname so you can also run the command:
   ```bash
   ./chat-client -user<USERNAME> -server<HOSTNAME>
   ```
5. Once the UI is initialized, you can type a message of upto 80 characters to the server which will be broadcasted to every client connected including yourself.
6. All messages being sent and received will have the direction symbols `>>` or `<<` indicating sent or received messages and they wil also include their timestamps.
7. To close the client application connection to the server and quit. You can simple type and send the message `>>bye<<`.

## Chat-server
The chat-server application is a standard server that mediates the communication between clients. It handles client connections and messages in a multithreaded environment.
### How to use:
1. The server application is built using the following command:
   ```bash
   make all
   ```
   Ensure that this command is run in the `bin` directory of the server application.
2. Once the server application binary is built you can run it using the command:
   ```bash
   ./chat-server
   ```
   Ensure that this also executed in the `bin` directory of the server application.
3. Unlike the client, no arguments are to be passed onto the server for it to run.
4. While the server is running, a maximum of 10 clients can connect to it without being rejected.
## Directory structure
1. The directory structure for this application bundle is designed in such a way that you can directly build both the applications using the command:
   ```bash
   make all
   ```
   To be executed under the main directory containing both the server and client folders, under `/CanWeTalkSystem`.
2. There is a Common folder that includes shared headers and source files for both the client and servers and they are built when building either the client or server.

# Happy Chatting!
