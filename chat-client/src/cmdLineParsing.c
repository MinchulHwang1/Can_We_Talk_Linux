/*
 * Filename:    cmdLineParsing.c
 * Project:     CanWeTalkSystem 
 * By:          Salman Nouman, Minchul Hwang, Md Saiful Islam, Saje-Antoine Rose
 * Date:        April, 02, 2024
 * Description: This file contains functions related to parsing command line arguements in the CanWeTalk System
 */

#include "../inc/cmdLineParsing.h"

/*
 * Function:    displayUsage
 * Description: Displays the usage of the program
 * Parameters:  Void
 * Returns:     void
 */
void displayUsage() 
{
    printf("Usage: chat-client -user<USERNAME> -server<SERVERNAME/IPADDRESS>\n");
}

/*
 * Function:    parseCommandLineArgs
 * Description: Retrives the command line arguements from the user and puts their values into a struct
 * Parameters:  int argc: The number of arguments provided
 *              char* argv: The arguement in question
 *              ClientArgs* clientArgs: A struct to hold the extrapolated value behind the arguements
 * Returns:     int: Error or success code
 */
int parseCommandLineArgs(int argc, char* argv[], ClientArgs* clientArgs) 
{
    // Initializing
    clientArgs->userName = NULL;
    clientArgs->serverName = NULL;
    clientArgs->ipAddress = NULL;

    const int kFirstCharacter = 0;
    const int kSecondCharacter = 1;
    const int kCmdFlagPlacement = 2; // Used in strncmp to search for first 2 chars

    for (int counter = kSecondCharacter; counter < argc; counter++) 
    {
        if (strncmp(argv[counter], "-user", kCmdFlagPlacement) == 0) 
        {
            clientArgs->userName = strchr(argv[counter], 'r') + kSecondCharacter; // removing the flag
        } 
        else if (strncmp(argv[counter], "-server", kCmdFlagPlacement) == 0) 
        {
            char* serverArg = strstr(argv[counter], "server") + strlen("server"); // Get pointer to the substring after "server"
            if (strlen(serverArg) > 0 && serverArg[kFirstCharacter] >= ASCII_DIGIT_START && serverArg[kFirstCharacter] <= ASCII_DIGIT_END) 
            {
                clientArgs->ipAddress = serverArg;
            } 
            else 
            {
                clientArgs->serverName = serverArg;
            }
        }
        else 
        {
            printf("Error: Invalid argument: %s\n", argv[counter]);
            displayUsage();
            return CMD_PARSING_ERROR; // Error
        }
    }

    // Check if both server name and IP address are provided
    if ((clientArgs->serverName && clientArgs->ipAddress) || (!clientArgs->serverName && !clientArgs->ipAddress)) 
    {
        printf("Error: Please provide either server name or IP address\n");
        displayUsage();
        return CMD_PARSING_ERROR; // Error
    }

    return 0; // Success
}