/*
* FILE              :   client-manager.h
* PROJECT           :   SENG2030 - A04 - CanWeTalkSystem
* PROGRAMMER        :   ​Md Saiful Islam, ​Minchul Hwang, ​Salman Nouman, ​Saje-Antonie Rose
* FIRST VERSION     :   2024-04-02 
* DESCRIPTION       :   This header file contains the global variables and function declarations for 
                        client-manager.c file.
*/

#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#define MAX_CLIENTS 10
#define SOCKET_ERROR -1
#define ONE_HUNDRED_MILLISECONDS 100000
#define STRING_EQUALITY 0
#define READING_ERROR 0
#define PORT_NUMBER 8989
extern int client_sockets[MAX_CLIENTS];
extern pthread_mutex_t clientsMutex;
extern pthread_mutex_t numClientsMutex;
extern int clientCount;
void init_client_manager();
bool add_client(int client_socket);
void remove_client(int client_socket);
void cleanup_clients();

#endif
