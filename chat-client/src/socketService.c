/*
 * Filename:    socketService.c
 * Project:     CanWeTalkSystem 
 * By:          Salman Nouman, Minchul Hwang, Md Saiful Islam, Saje-Antoine Rose
 * Date:        April, 02, 2024
 * Description: This file contains functionality to handle the socket opertations with the server on the client end.
 */

#include "../inc/socketService.h"


/*
 * Function:    resolveServerName
 * Description: Resolves the given name of the server to an actual ip address
 * Parameters:  char* serverName: The given name of a server
 *              char* ipAddress: The buffer to contain the ip address
 * Returns:     void
 */
void resolveServerName(char *serverName, char* ipAddress) 
{
    struct hostent *hostEntry;
    struct in_addr **addrList;
    
    if ((hostEntry = gethostbyname(serverName)) == NULL) 
    {
        herror("gethostbyname");
        exit(EXIT_FAILURE);
    }

    addrList = (struct in_addr **)hostEntry->h_addr_list;
    if (addrList[FIRST_IP_ADDY_IN_LIST] == NULL) 
    {
        fprintf(stderr, "No IP address found for server name: %s\n", serverName);
        exit(EXIT_FAILURE);
    }

    // Copy IP address to buffer
    const char* ipAddressStr = inet_ntoa(*addrList[FIRST_IP_ADDY_IN_LIST]);
    size_t ipAddressLength = strlen(ipAddressStr);
    if (ipAddressLength >= MAX_IP_LENGTH) 
    {
        fprintf(stderr, "IP address exceeds maximum length: %s\n", ipAddressStr);
        exit(EXIT_FAILURE);
    }

    strcpy(ipAddress, ipAddressStr);
}


/*
 * Function:    connectToServer
 * Description: This function connects the client to the server via the use of sockets
 * Parameters:  char* serverIp: The IP address of the server.
 * Returns:     int: The socket descriptor or an error value
 */
int connectToServer(char *serverIP) 
{
    int socketDescriptor;

    // Create a socket for communication
    if ((socketDescriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        perror("Socket creation failed");
        return SOCKET_ERROR;
    }

    // Initialize server address structure
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT_NUMBER);

    // Convert IP address from string to network byte order
    if (inet_pton(AF_INET, serverIP, &serverAddress.sin_addr) <= 0) 
    {
        perror("Invalid address/ Address not supported");
        close(socketDescriptor);
        return SOCKET_ERROR;
    }

    // Connect to the server
    if (connect(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) 
    {
        perror("Connection failed");
        close(socketDescriptor);
        return SOCKET_ERROR;
    }

    return socketDescriptor;
}

/*
 * Function:    initializeConnection
 * Description: This function can be used to start a connection to a server
 * Parameters:  const ClientArgs *clientArgs: The related information about a server that the client wishes to connect to
 * Returns:     int: The socket descriptor or an error value which is retrived from connectToServer
 */
int initializeConnection(const ClientArgs *clientArgs) 
{
    char resolvedIPAddress[MAX_IP_LENGTH] = {0};
    if (clientArgs->serverName != NULL) 
	{
        resolveServerName(clientArgs->serverName, resolvedIPAddress);
    } 
	else 
	{
        strncpy(resolvedIPAddress, clientArgs->ipAddress, MAX_IP_LENGTH - 1);
    }
    return connectToServer(resolvedIPAddress);
}

/*
 * Function:    getSocketIP
 * Description: This function can be used to retrive the IP address for a socket
 * Parameters:  int sockfd: The socketDescriptor retrived from connectToServer
 *              char* ipBuffer: A buffer to hold the IP
 *              size_t bufferLength: Buffer size.
 * Returns:     void.
 */
void getSocketIP(int sockfd, char *ipBuffer, size_t bufferLength) 
{
    struct sockaddr_in localAddress;
    socklen_t addressLength = sizeof(localAddress);

    if (getsockname(sockfd, (struct sockaddr*)&localAddress, &addressLength) == -1) 
	{
        perror("unable to get socket name");
        exit(EXIT_FAILURE);
    }

    // Convert IP to string
    const char *ip = inet_ntoa(localAddress.sin_addr);

    // Copy to provided buffer
    strncpy(ipBuffer, ip, bufferLength);
    ipBuffer[bufferLength - 1] = '\0'; // Ensure null-termination
}