/*
 * Filename:    main.c
 * Project:     CanWeTalkSystem 
 * By:          Salman Nouman, Minchul Hwang, Md Saiful Islam, Saje-Antoine Rose
 * Date:        April, 02, 2024
 * Description: This file contains logic and function calls that integrates all the auxillary files in the chat-client system 
 * 				in order to provide the functionality of a chat client.
 */

#include"../inc/socketService.h"
#include "../inc/clientThreads.h"

// Initializing global shared resources
pthread_mutex_t listenerMutex = PTHREAD_MUTEX_INITIALIZER;
int terminateListener = 0;

int main(int argc, char *argv[]) 
{
	// Parse command-line arguments
    ClientArgs clientArgs;
    int parseResult = parseCommandLineArgs(argc, argv, &clientArgs);
    if (parseResult != CMD_PARSING_SUCCESS) 
    {
		pthread_mutex_destroy(&listenerMutex);
        return parseResult;
    }
    // Initialize connection to server
    int connectionResult = initializeConnection(&clientArgs);
    if (connectionResult == SOCKET_ERROR) 
	{
		pthread_mutex_destroy(&listenerMutex);
        return SOCKET_ERROR;
    }
	
	// Storing client IP for use later
	char clientIp[MAX_IP_LENGTH];
	getSocketIP(connectionResult, clientIp, MAX_IP_LENGTH);

    int rows, cols;
    WINDOW *staticMessagesHeader, *staticOutgoingHeader, *messageWindow, *outgoingWindow;
    initializeUI(&rows, &cols, &staticMessagesHeader, &staticOutgoingHeader, &messageWindow, &outgoingWindow);

	pthread_t listener;// Thread, queue and arguments initialization here
	pthread_t sender;
	MessageQueue incomingQueue;
	queueInit(&incomingQueue);
	// initialization of listener arguments
	ThreadArgs listenerArgs;
	listenerArgs.serverSocket = connectionResult;
	listenerArgs.queue = &incomingQueue;
	// initialization of sender arguments 
	ThreadArgs senderArgs;
	senderArgs.serverSocket = connectionResult;
	senderArgs.window = outgoingWindow;
	senderArgs.ip = clientIp;
	senderArgs.userName = clientArgs.userName;

	// Start the listener thread
	if (pthread_create(&listener, NULL, listenerThread, (void *)&listenerArgs) != 0) 
	{
    	perror("Failed to create listener thread");
		close(connectionResult);
		freeQueue(&incomingQueue);
		pthread_mutex_destroy(&listenerMutex);
		endwin();
    	return 1;
	}

	// Start the sender thread
	if (pthread_create(&sender, NULL, senderThread, (void *)&senderArgs) != 0) 
	{
    	perror("Failed to create listener thread");
    	return 1;
	}

	while (true) // Main UI thread here
	{
		Message incomingMessage;// Get message from server here
		while (dequeue(&incomingQueue, &incomingMessage))
		{
			printMessage(messageWindow, &incomingMessage, clientIp);
		}

		pthread_mutex_lock(&listenerMutex);
		int terminate = terminateListener;
		pthread_mutex_unlock(&listenerMutex);
		if (terminate)
		{
			break;
		}

	}

	// Clean up resources, threads, and ncurses UI
	pthread_join(listener, NULL);
	pthread_join(sender, NULL);
	close(connectionResult);
	pthread_mutex_destroy(&listenerMutex);
	freeQueue(&incomingQueue);
    endwin();

    return 0;
}