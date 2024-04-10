/*
 * Filename:    cmdLineParsing.h
 * Project:     CanWeTalkSystem 
 * By:          Salman Nouman, Minchul Hwang, Md Saiful Islam, Saje-Antoine Rose
 * Date:        April, 02, 2024
 * Description: This file contains defined values, dependencies and prototypes for functions related to parsing command line arguements in the CanWeTalk System
 */


#ifndef CMDLINEPARSING_H
#define CMDLINEPARSING_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

#define CMD_PARSING_ERROR -1
#define CMD_PARSING_SUCCESS 0

#define ASCII_DIGIT_START '0'
#define ASCII_DIGIT_END '9'

// Structure to store parsed command-line arguments
typedef struct 
{
    char* userName;
    char* serverName;
    char* ipAddress;
} ClientArgs;

int parseCommandLineArgs(int argc, char* argv[], ClientArgs* clientArgs);

#endif