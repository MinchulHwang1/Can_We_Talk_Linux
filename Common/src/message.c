/*
 * Filename:    message.c
 * Project:     CanWeTalkSystem 
 * By:          Salman Nouman, Minchul Hwang, Md Saiful Islam, Saje-Antoine Rose
 * Date:        April, 02, 2024
 * Description: This file contains functionality to handle the message operations.
 */


#include "../inc/message.h"

/*
 * Function:    sendLengthPrefixedMessage
 * Description: This function is responsible for sending a length-prefixed message over a socket connection.
 * Parameters:  const char* chunk: Message content which is sent in a chunk
 *              int socketConnection: The socket file descriptor
 * Returns:     void
 */
void sendLengthPrefixedMessage(const char* chunk, int socketConnection)
{
    uint32_t msgLength = htonl(strlen(chunk)); // Convert message length to network byte order
    // Send the length of the message
    if (send(socketConnection, &msgLength, sizeof(msgLength), 0) < 0) 
	{
        perror("send");
        exit(EXIT_FAILURE);
    }

    // Send the message itself
    if (send(socketConnection, chunk, strlen(chunk), 0) < 0) 
	{
        perror("send");
        exit(EXIT_FAILURE);
    }
}

/*
 * Function:    sendParcelledMessage
 * Description: This function is responsible for sending a large message in smaller chunks over a socket connection
 * Parameters:  const Message* chatMessage: A pointer to a Message structure containing information about the message to be sent.
 *              int socketConnection: The socket file descriptor
 * Returns:     void
 */
void sendParcelledMessage(const Message* chatMessage, int socketConnection) 
{
    int messageLength = strlen(chatMessage->message);
	char serializedMessage[MAX_SERIALIZED_LENGTH];
    int begin = 0; // begin index of the current chunk
    char parcel[MAX_PARCEL_LENGTH];

    while (begin < messageLength) 
	{
		memset(parcel, 0, sizeof(parcel)); // Initialize buffers for use
		memset(serializedMessage, 0, sizeof(serializedMessage)); 

        int chunkLength = MAX_PARCEL_LENGTH - 1;
        if (begin + chunkLength > messageLength) 
		{
            chunkLength = messageLength - begin; // Adjust for the last chunk
        } 
		else 
		{
            // Ensure we don't split in the middle of a word
            while (chatMessage->message[begin + chunkLength] != ' ' && chunkLength > 0) 
			{
                chunkLength--;
            }
            if (chunkLength == 0) 
			{
                // If we didn't find a space, just use the max chunk size
                chunkLength = MAX_PARCEL_LENGTH - 1;
            }
        }
		
        strncpy(parcel, chatMessage->message + begin, chunkLength);
        parcel[chunkLength] = '\0'; // Null terminate the parcel

		serializeMessage(chatMessage, parcel, serializedMessage, MAX_SERIALIZED_LENGTH);

        sendLengthPrefixedMessage(serializedMessage, socketConnection);

        begin += chunkLength;
        if (chatMessage->message[begin] == ' ') 
		{
            begin++; // Skip the space at the beginning of the next chunk
        }
    }
}


/*
 * Function:    getCurrentTimestamp
 * Description: This function gets the current time, formats it accordingly and puts it into a buffer
 * Parameters:  char* buffer: The buffer tohold the timestamp
 *              size_t bufSize: The size of the buffer
 * Returns:     void
 */
void getCurrentTimestamp(char* buffer, size_t bufSize)
{
	time_t now = time(NULL);
	struct tm *localNow = localtime(&now);
	strftime(buffer, bufSize, "%H:%M:%S", localNow);
}


/*
 * Function:    serializeMessage
 * Description: This function sereializes a chats message in order to be sent over a network. It uses a | charachter as a 
 *              delianiater between the messages content
 * Parameters:  const Message* chatMessage: A pointer to a Message structure containing information about the message to be sent.
 *              char* parcel: The messages content
 *              char* serializedMessage: A buffer to store the serialized message
 *              size_t bufSize: The size of the buffer
 * Returns:     void
 */
void serializeMessage(const Message* chatMessage, char* parcel, char* serializedMessage, size_t bufferSize)
{
	    // Serializing message here using the '|' character
    snprintf(serializedMessage, bufferSize, "%s|%s|%s",
             chatMessage->ip, chatMessage->userName, parcel);
}


/*
 * Function:    deserializeMessage
 * Description: This function takes a serialized message and deserialiazes it and extracts the related message components into a Message structure
 *              delianiater between the messages content
 * Parameters:  Message* chatMessage: A pointer to a Message structure containing information about the message sent.
 *              const char* serializedMessage: A buffer to store the serialized message
 * Returns:     void
 */
void deserializeMessage(Message* chatMessage, const char* serializedMessage) 
{
    char *token;
    char *rest = strdup(serializedMessage); // Duplicating to not modify the original
    char *originalRest = rest; // Memory pointer for freeing later

    // Extract IP
    token = strtok_r(rest, "|", &rest);
    if (token) 
	{
		strncpy(chatMessage->ip, token, MAX_IP_LENGTH);
	}	
    chatMessage->ip[MAX_IP_LENGTH - 1] = '\0'; // Ensure null-termination

    // Extract Username
    token = strtok_r(NULL, "|", &rest);
    if (token)
	{
		strncpy(chatMessage->userName, token, MAX_USERNAME_LENGTH);
	}
    chatMessage->userName[MAX_USERNAME_LENGTH - 1] = '\0';

    // Extract Message
    token = strtok_r(NULL, "|", &rest);
    if (token)
	{
		strncpy(chatMessage->message, token, MAX_PARCEL_LENGTH);
	}
    chatMessage->message[MAX_PARCEL_LENGTH - 1] = '\0';

    free(originalRest); // Free the duplicated string
}
