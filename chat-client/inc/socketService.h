/*
 * Filename:    socketService.h
 * Project:     CanWeTalkSystem 
 * By:          Salman Nouman, Minchul Hwang, Md Saiful Islam, Saje-Antoine Rose
 * Date:        April, 02, 2024
 * Description: This file contains the defined values, dependencies and function prototypes to enable the use of sockets in the chat-client
 */

#ifndef SOCKET_SERVICE_H
#define SOCKET_SERVICE_H

#include "cmdLineParsing.h"

#define MAX_IP_LENGTH 16 // Maximum length of an IPv4 address (including null terminator)
#define PORT_NUMBER 8989
#define SOCKET_ERROR -1
#define SOCKET_SUCCESS 0
#define FIRST_IP_ADDY_IN_LIST 0

int initializeConnection(const ClientArgs *clientArgs);
void resolveServerName(char *serverName, char* ipAddress);
int connectToServer(char *serverIP);
void getSocketIP(int sockfd, char *ipBuffer, size_t bufferLength);

#endif