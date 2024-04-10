/*
* FILE              :   server-utility.h
* PROJECT           :   SENG2030 - A04 - CanWeTalkSystem
* PROGRAMMER        :   ​Md Saiful Islam, ​Minchul Hwang, ​Salman Nouman, ​Saje-Antonie Rose
* FIRST VERSION     :   2024-04-02 
* DESCRIPTION       :   This header file contains the global variables and function declarations for 
                        server-utility.c file.
*/

#include "../../Common/inc/queue.h"
#include "../inc/client-manager.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <signal.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>

extern volatile sig_atomic_t runServer;
extern pthread_t thread_id[MAX_CLIENTS];
extern pthread_t broadcaster_tid;
extern MessageQueue messageQueue;
extern int sockfd;
// Function prototype for the thread that handles connections
void* connection_handler(void* socket_desc);
void signalHandler(int sig);
void* broadcasterThread(void* arg);
int init_server_socket(int port);
void close_socket(int sock);
void* connection_handler(void* socket_desc);
void* broadcasterThread(void* arg);
int accept_client_connection(int server_socket);
void serverShutdown(void);


