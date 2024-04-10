/*
* FILE              :   main.c
* PROJECT           :   SENG2030 - A04 - CanWeTalkSystem
* PROGRAMMER        :   ​Md Saiful Islam, ​Minchul Hwang, ​Salman Nouman, ​Saje-Antonie Rose
* FIRST VERSION     :   2024-04-02 
* DESCRIPTION       :   This file contains the main function for the chat-server program. 
*/

////////////////////////////////////
/*
	SERVER DOCUMENTATION:
	
	MESSAGE PROTOCOL:
	
	The Communication protocol was designed to be simple for an application that is called ChatLite. The client sends a length
	prefixed message that has the length of the message itself as an integer, and the message itself afterwards. This ensures that
	the server knows how long the message is that is being sent. The client serializes the message sent delimited with the '|' symbol
	to ease the deserialization process. The server receives the length prefixed message where the length of the message is first received
	and then a buffer is dynamically allocated based on the message length received. It is then deserialized and passed onto a shared queue
	for processing.

	Within the message. The client sends its IP, message and the username. the format is as follows:
										ip|username|message

	When deserializing the message, This information is stored into a struct 'Message' which is pushed into the queue for processing in the
	broadcaster thread.

	When a client sends a >>bye<< message, the server reads it and ensures the thread that handles the specific client is cleaned up. It also
	makes sure that the spot taken up by the client is also vacated for other clients to connect. The data structure of the array is specified
	below. A 'Message' struct ensures the right data is broadcast to all the clients including the ip address of the sending client.

	DATA STRUCTURE:

	The server uses a simple array that is initialzied to -1 for all the values, it stores upto 10 client sockets that are setup when a client
	successfully connects. The server spawns a broadcaster thread to handle any messages that are being sent by any client that is connected.
	Similar to the client, the server also spawns a handler thread to ensure communication is not blocked when the client connects. A shared queue
	is used to ensure the messages are being processed while the connection handler threads receive the messages.

    CLEAN UP PROCEDURE: 
    The server employs a comprehensive cleanup procedure to ensure graceful shutdown and resource management. This process is 
    triggered by either the SIGINT or SIGTERM signals, allowing the server to shutdown gracefully in response to external 
    interrupts or requests.

    1. Joining Client Handler Threads:
       The server maintains a list of threads, each corresponding to a connected client. Upon shutdown, the server joins all these 
       threads, ensuring that they have completed their execution. 
    2. Terminating the Broadcaster Thread:
       A dedicated broadcaster thread is responsible for relaying messages from the shared message queue to all connected clients. 
       During the cleanup, this thread is signaled to terminate (using pthread_cancel), and then joined to ensure it has ceased execution.
    3. Client Cleanup:
       All active client connections are gracefully closed. The server iterates through the array of client sockets, closing each 
       socket and marking the slot as available. This step ensures that all network resources are properly released and clients 
       are informed of the server shutdown.
    4. Closing the Server Socket:
       The main server socket, which listens for incoming connections, is closed. This prevents any new client connections from 
       being accepted during the shutdown process.
    5. Clearing the Message Queue:
       The server maintains a queue of messages to be broadcasted. During shutdown, this queue is emptied and freed, 
       ensuring that no memory leaks occur from leftover messages.

*/
////////////////////////////////////

#include "../../Common/inc/queue.h"
#include "../inc/client-manager.h"
#include "server-utility.h"

// global variables, flags and other shared resources initialized here
MessageQueue messageQueue;
volatile sig_atomic_t runServer = 1;
pthread_mutex_t clientsMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t numClientsMutex = PTHREAD_MUTEX_INITIALIZER;
int clientCount = 0;
pthread_t thread_id[MAX_CLIENTS];
int client_sockets[MAX_CLIENTS];
pthread_t broadcaster_tid;
int sockfd;
void serverShutdown(void);

int main(int argc, char** argv)
{
    init_client_manager();
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    sockfd = init_server_socket(PORT_NUMBER);
    
    int client_index = 0;
    queueInit(&messageQueue);

    

    // Start the broadcaster thread
    if (pthread_create(&broadcaster_tid, NULL, broadcasterThread, NULL) != 0) 
    {
        perror("Failed to create broadcaster thread");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    while (runServer) 
    {	
        int newsockfd;
        pthread_mutex_lock(&numClientsMutex);	
        int client_count =  clientCount;
        pthread_mutex_unlock(&numClientsMutex);
        if (client_count < MAX_CLIENTS)
        {
            if ((newsockfd = accept_client_connection(sockfd)) == SOCKET_ERROR)
            {
                break;
            }
            if (!add_client(newsockfd)) 
            {
                printf("Maximum number of clients reached. Rejecting new connection.\n");
                close(newsockfd);
            }
            else
            {
                pthread_mutex_lock(&numClientsMutex);
                clientCount++;
                pthread_mutex_unlock(&numClientsMutex);
                // Allocate memory for a new socket descriptor
                int* new_sock = malloc(sizeof(int));
                if (!new_sock) 
                {
                    perror("malloc failed");
                    continue;
                }

                *new_sock = newsockfd;

                // Create a thread for each connection
                if (pthread_create(&thread_id[client_index], NULL, connection_handler, (void*)new_sock) < 0) 
                {
                    perror("could not create thread");
                    close(newsockfd);
                    free(new_sock);
                    continue;
                }
            }
        }
    }

    serverShutdown();

    return 0;
}

