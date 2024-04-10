/*
* FILE              :   server-utility.c
* PROJECT           :   SENG2030 - A04 - CanWeTalkSystem
* PROGRAMMER        :   ​Md Saiful Islam, ​Minchul Hwang, ​Salman Nouman, ​Saje-Antonie Rose
* FIRST VERSION     :   2024-04-02 
* DESCRIPTION       :   This file contains the functions that help the server operate and handle connections
                        from clients. 
*/

#include "server-utility.h"
/*
 * Function:    signalHandler
 * Description: This function retrives and interperets a signal and acts accordingly based upon what the required response to the signal is.
 *              It cleans up and closes the server socket and message queue
 * Parameters:  int sig: The signal
 * Returns:     void
 */
void signalHandler(int sig)
{
    if(sig == SIGINT)
    {
        runServer = 0;
    }
    else if(sig == SIGTERM)
    {
        runServer = 0;
    }

    // shut down server and clean up resources
    serverShutdown();
    exit(EXIT_SUCCESS);
}


/*
 * Function:    init_server_socket
 * Description: This function creates and initializes a socket that listens for incomming connection on a specified port.
 * Parameters:  int port: The port number that the socket should be set up to listen on
 * Returns:     int: Error value if necessary else the socket file descriptor
 */
int init_server_socket(int port)
{

    int sockfd;
    struct sockaddr_in server_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // check for failure
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return SOCKET_ERROR;
    }

    // zero out the memory block occupied by the server_addr structure
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // listen for incoming TCP/IP connections on any IPv4 address
    
    server_addr.sin_port = htons(port); // listen on port. htons converts port number to network byte
                                        // order (big endian)

    // Bind the socket to the server address and port
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error in binding");
        close(sockfd);
        return SOCKET_ERROR;
    }
    printf("bind successful\n");
    // Start listening on the socket for connections, with a maximum of 10 pending connections
    if (listen(sockfd, 5) < 0) // Check if listen call was successful
    {
        perror("Error on listening");
        return SOCKET_ERROR;
    }
    printf("Server initialized and listening on port %d\n", port);
    return sockfd;
}


/*
 * Function:    accept_client_connection
 * Description: This function accepts incoming connections on a specific socket. it creates a new socket for each connection with a client
 *              which allows for bi-directional communication
 * Parameters:  int server_socket: The socket file descriptor to accept connections on
 * Returns:     int: Error value if necessary else a new socket file descriptor
 */
int accept_client_connection(int server_socket) 
{
    struct sockaddr_in client_addr;
    socklen_t clilen = sizeof(client_addr);

    // Wait for a client connection
    int newsockfd = accept(server_socket, (struct sockaddr *) &client_addr, &clilen);

    if (newsockfd < 0) 
    {
        perror("Error on accept");
        return SOCKET_ERROR;
    }

    printf("Client connected from IP: %s\n", inet_ntoa(client_addr.sin_addr));

    return newsockfd; // Return the new socket file descriptor
}


/*
 * Function:    close_socket
 * Description: This function attempts to close a socket via its socket file descriptor. If the socket is invalid it logs an error
 * Parameters:  int sock: The socket file descriptor.
 * Returns:     void
 */
void close_socket(int sock) 
{
    if (sock >= 0) 
    {
        if (close(sock) == -1) 
        {
            perror("Failed to close socket");
        } 
    }
}

/*
 * Function:    connection_handler
 * Description: This function recives the messages from the clients and, allocate memory for it, deserializes it 
 *              and checks to see if the client wishes to disconnect via the ">>bye<<"" keyword
 * Parameters:  void* socket_desc: pointer to int representing socket file description
 * Returns:     void
 */
void* connection_handler(void* socket_desc)
{
    // unwrap the socket object
    int sock = *(int*)socket_desc;
    Message* chatMessage = NULL;
    char* buffer = NULL;
    while (runServer)
    {
		uint32_t msgLength;
		// Receive the length of the message
		if (recv(sock, &msgLength, sizeof(msgLength), 0) < READING_ERROR) 
		{
			perror("recv");
		}

		msgLength = ntohl(msgLength); // Convert message length to host byte order

		buffer = malloc(msgLength + 1); // Allocate memory for the message
		if (buffer == NULL) 
		{
			perror("malloc failed");
			exit(EXIT_FAILURE);
		}

		// Receive the message itself
		if (recv(sock, buffer, msgLength, 0) < READING_ERROR) 
		{
			perror("recv");
		}

		buffer[msgLength] = '\0'; // Null-terminate the string	
        // Terminate the received string

        chatMessage = (Message*)malloc(sizeof(Message));
        if (!chatMessage) 
        {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }

        deserializeMessage(chatMessage, buffer);
		if(strcmp(chatMessage->message, ">>bye<<") == STRING_EQUALITY)
		{
            // when client sent '>>bye<<' message, quit
            puts("Client disconnected");
            fflush(stdout);
            remove_client(sock);
            pthread_mutex_lock(&numClientsMutex);
            clientCount--;
            pthread_mutex_unlock(&numClientsMutex);
			break;
		}
        chatMessage->senderSock = sock;
        enqueue(&messageQueue, chatMessage);
    }

    close(sock);
    free(socket_desc);
    free(chatMessage);
    free(buffer);
    return NULL;
}

/*
 * Function:    broadcasterThread
 * Description: This function is responsible for broadcasting the messages to the connected clients. 
 *              It dequeues messages from a message queue and sends them to their corresponding connected clients
 * Parameters:  void.
 * Returns:     void
 */
void* broadcasterThread(void* arg) 
{
    while (runServer)
    {
        Message message;
        if (dequeue(&messageQueue, &message)) 
        {
            pthread_mutex_lock(&clientsMutex);
            for (int i = 0; i < MAX_CLIENTS; ++i) 
            {
                if (client_sockets[i] != -1) 
                {
                    // Serialize and send the message to the client
                    char serializedMessage[MAX_SERIALIZED_LENGTH];
                    serializeMessage(&message, message.message, serializedMessage, sizeof(serializedMessage));
					sendLengthPrefixedMessage(serializedMessage, client_sockets[i]);
                }
            }
            pthread_mutex_unlock(&clientsMutex);
        }
        // Implementing sleep to avoid busy waiting
        usleep(ONE_HUNDRED_MILLISECONDS); // Sleep for 100ms
    }
    return NULL;
}


/*
 * Function:    serverShutdown
 * Description: This function is responsible for shutting down the server and cleaning up resources and closing threads.
 * Parameters:  void.
 * Returns:     void
 */
void serverShutdown(void)
{
    // Server shutdown procedure
    // Join all client handler threads
    for (int i = 0; i < MAX_CLIENTS; i++) 
    {
        if (thread_id[i]) 
        {
            pthread_join(thread_id[i], NULL);
        }
    }

    // Cancel and join the broadcaster thread
    pthread_cancel(broadcaster_tid);
    pthread_join(broadcaster_tid, NULL);

    cleanup_clients();

    // Close the server socket
    close_socket(sockfd);

    // Clean up the message queue
    freeQueue(&messageQueue);
}