/*
 * Filename:    clientThreads.c
 * Project:     CanWeTalkSystem 
 * By:          Salman Nouman, Minchul Hwang, Md Saiful Islam, Saje-Antoine Rose
 * Date:        April, 02, 2024
 * Description: This file contains the logic behind functions that handle thread operations in the chat client
 */

#include "../inc/clientThreads.h"


/*
 * Function:    *listenerThread
 * Description: This function listens for incoming messages on a server socket.
 * Parameters:  void *threadArgs: Used to recive a pointer to a structure
 * Returns:     void
 */
void *listenerThread(void *threadArgs) 
{
    ThreadArgs *args = (ThreadArgs *)threadArgs;
    int serverSocket = args->serverSocket;
	MessageQueue* queue = args->queue;
	Message* chatMessage = (Message*)malloc(sizeof(Message));

    while(true) 
	{
        pthread_mutex_lock(&listenerMutex);
        int terminate = terminateListener; // Acquire lock and check global condition
        pthread_mutex_unlock(&listenerMutex);

        if (terminate) 
		{
            break;
        }

		uint32_t msgLength;
		// Receive the length of the message
		if (recv(serverSocket, &msgLength, sizeof(msgLength), 0) == 0) 
		{
			break;
		}

		msgLength = ntohl(msgLength); // Convert message length to host byte order

		char* buffer = malloc(msgLength + 1); // Allocate memory for the message
		if (buffer == NULL) 
		{
			perror("malloc failed");
			exit(EXIT_FAILURE);
		}

		// Receive the message itself
		if (recv(serverSocket, buffer, msgLength, 0) == 0) 
		{
			break;
		}

		buffer[msgLength] = '\0'; // Null-terminate the string	


		deserializeMessage(chatMessage, buffer);
		getCurrentTimestamp(chatMessage->timeStamp, MAX_TIMESTAMP_LENGTH);
		enqueue(queue, chatMessage);
		free(buffer);
    }
	
	pthread_mutex_lock(&listenerMutex);
    terminateListener = 1; // Acquire lock and Set global condition
    pthread_mutex_unlock(&listenerMutex);
	free(chatMessage);
    return NULL;
}

/*
 * Function:    *listenerThread
 * Description: This function handles user input and sends messages to a server.
 * Parameters:  void *threadArgs: Used to recive a pointer to a struct
 * Returns:     void
 */
void *senderThread(void *threadArgs) 
{
	ThreadArgs *args = (ThreadArgs *)threadArgs;
	WINDOW* chatWindow = args->window;
    int serverSocket = args->serverSocket;
	char *ip = args->ip;
	char* userName = args->userName;

	char userInput[MAX_MESSAGE_LENGTH];
	memset(userInput, '\0', sizeof(userInput));// Clear buffer

	while (true) 
	{
		// Clear the previous input
		werase(chatWindow);
		wrefresh(chatWindow); // Refresh to show the changes

		// Print the prompt again after clearing
		mvwaddch(chatWindow, ISTREAM_START, 0, '>');
		wmove(chatWindow, ISTREAM_START, NEXT_COLUMN + NEXT_COLUMN);
		wrefresh(chatWindow);

		// Turn on character echo
		echo();
		// Read the user input upto 79 chars for null terminator
		wgetnstr(chatWindow, userInput, 79);
		// Turn off character echo
		noecho();

		pthread_mutex_lock(&listenerMutex);
        int terminate = terminateListener; // Acquire lock and check global condition
        pthread_mutex_unlock(&listenerMutex);

		if (terminate) 
		{
            break;
        }

		if (strlen(userInput) > 0)
		{
			// Build message struct before sending
			Message *outMessage = (Message*)malloc(sizeof(Message));
			strcpy(outMessage->ip, ip);
			strncpy(outMessage->userName, userName, MAX_USERNAME_LENGTH - 1);
			outMessage->userName[MAX_USERNAME_LENGTH - 1] = '\0'; // Ensure null-termination
			strcpy(outMessage->message, userInput);
			getCurrentTimestamp(outMessage->timeStamp, MAX_TIMESTAMP_LENGTH);

			sendParcelledMessage(outMessage, serverSocket);

			if (strcmp(userInput, ">>bye<<") == 0)// in the case of >>bye<<, end client and its threads
			{
				pthread_mutex_lock(&listenerMutex);
				terminateListener = 1;
				pthread_mutex_unlock(&listenerMutex);
				free(outMessage);
				break;
			}

			free(outMessage);
		}
		memset(userInput, '\0', sizeof(userInput));// Reset userInput buffer
	}

	return NULL;
}