/*
 * Filename:    message.h
 * Project:     CanWeTalkSystem 
 * By:          Salman Nouman, Minchul Hwang, Md Saiful Islam, Saje-Antoine Rose
 * Date:        April, 02, 2024
 * Description: This file contains the defined values, dependencies and function prototypes to handle messages in the CanWeTalkSystem
 */

#ifndef MESSAGE_H
#define MESSAGE_H

#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>


#define MAX_PARCEL_LENGTH 41
#define MAX_MESSAGE_LENGTH 81
#define MAX_USERNAME_LENGTH 6
#define MAX_TIMESTAMP_LENGTH 9
#define MAX_SERIALIZED_LENGTH 63
#define MAX_IP_LENGTH 16

typedef struct Message
{
	char ip[MAX_IP_LENGTH];
	char userName[MAX_USERNAME_LENGTH];
	char message[MAX_MESSAGE_LENGTH];
	char timeStamp[MAX_TIMESTAMP_LENGTH];
	int senderSock;
} Message;

void sendLengthPrefixedMessage(const char* chunk, int socketConnection);
void sendParcelledMessage(const Message* chatMessage, int socketConnection);
void serializeMessage(const Message* chatMessage, char* parcel, char* serializedMessage, size_t bufferSize);
void deserializeMessage(Message* chatMessage, const char* serializedMessage);
void getCurrentTimestamp(char* buffer, size_t bufSize);

#endif