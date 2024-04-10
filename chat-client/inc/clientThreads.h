/*
 * Filename:    clientThreads.h
 * Project:     CanWeTalkSystem 
 * By:          Salman Nouman, Minchul Hwang, Md Saiful Islam, Saje-Antoine Rose
 * Date:        April, 02, 2024
 * Description: This file contains the defined values, dependencies and function prototypes to enable the use of threads in the chat-client
 */

#ifndef CLIENT_THREADS_H
#define CLIENT_THREADS_H

#include "ui.h"
#include "../../Common/inc/queue.h"

// Global Mutexes for UI resources here
extern pthread_mutex_t listenerMutex;
extern int terminateListener;

// structs here
typedef struct ThreadArgs
{
	WINDOW* window;
	int serverSocket;
	MessageQueue* queue;
	char* ip;
	char* userName;
} ThreadArgs;

void *listenerThread(void *threadArgs);
void *senderThread(void *threadArgs);


#endif