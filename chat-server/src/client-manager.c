/*
* FILE              :   client-manager.c
* PROJECT           :   SENG2030 - A04 - CanWeTalkSystem
* PROGRAMMER        :   ​Md Saiful Islam, ​Minchul Hwang, ​Salman Nouman, ​Saje-Antonie Rose
* FIRST VERSION     :   2024-04-02
* DESCRIPTION       :   This file contains the helper functions for the chat-server. 
*/

#include "../inc/client-manager.h"

/* 
    FUNCTION    :   init_client_manager
    DESCRIPTION :   This function initializes all elements of client_sockets array to -1
                    which indicates that the spot for the client socket is not taken.
    PARAMETERS  :   none
    RETURNS     :   void
*/
void init_client_manager() 
{
    pthread_mutex_lock(&clientsMutex);
    memset(client_sockets, -1, sizeof(client_sockets)); // Initialize all client sockets to -1
    pthread_mutex_unlock(&clientsMutex);
}


/* 
    FUNCTION    :   add_client
    DESCRIPTION :   Attempts to add a new client's socket to the server's array of client sockets.
                    The function searches for an available slot in the client_sockets array to store
                    the socket descriptor of a new client. It uses a mutex (clientsMutex) to ensure 
                    thread safety during the operation, making it safe to call in a multi-threaded 
                    environment like a server handling multiple client connections simultaneously.
    PARAMETERS  :   int client_socket
    RETURNS     :   bool
*/
bool add_client(int client_socket) 
{
    pthread_mutex_lock(&clientsMutex); // Lock the mutex to ensure thread-safe access to client_sockets array
    for (int i = 0; i < MAX_CLIENTS; i++) 
    {
        if (client_sockets[i] == -1) // Look for an empty slot
        { 
            client_sockets[i] = client_socket; // Add client socket to the array
            pthread_mutex_unlock(&clientsMutex); // Unlock the mutex before returning
            return true;
        }
    }
    pthread_mutex_unlock(&clientsMutex); // Unlock the mutex before returning
    return false; // No empty slot was found
}

/*
    FUNCTION    :   remove_client
    DESCRIPTION :   Removes a client's socket descriptor from the server's array of client sockets.
                    This function iterates through the client_sockets array to find the matching
                    client socket descriptor. Upon finding the descriptor, it sets its value in the
                    array to -1, indicating that the slot is now available for a new connection.
    PARAMETERS  :   int client_socket - The socket descriptor of the client to be removed. This
                    descriptor is used to identify the client in the array of connected clients.
    RETURNS     :   void - This function does not return a value.
*/
void remove_client(int client_socket) 
{
    pthread_mutex_lock(&clientsMutex); // Lock the mutex to ensure thread-safe access to client_sockets array
    for (int i = 0; i < MAX_CLIENTS; i++) 
    {
        if (client_sockets[i] == client_socket) 
        {
            client_sockets[i] = -1; // Remove client socket from the array and reset it to -1
            break; // Exit the loop once the client socket is found and handled
        }
    }
    pthread_mutex_unlock(&clientsMutex); // Unlock the mutex after operation
}

/*
    FUNCTION    :   cleanup_clients
    DESCRIPTION :   Iterates through the server's array of client sockets, closing any open socket
                    descriptors and marking their slots as available.
    PARAMETERS  :   None
    RETURNS     :   void 
*/
void cleanup_clients() 
{
    pthread_mutex_lock(&clientsMutex);
    for (int i = 0; i < MAX_CLIENTS; i++) 
    {
        if (client_sockets[i] != -1) {
            close(client_sockets[i]); // Close the socket
            client_sockets[i] = -1; // Mark as available
        }
    }
    pthread_mutex_unlock(&clientsMutex);
}
