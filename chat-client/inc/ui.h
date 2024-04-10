/*
 * Filename:    ui.h
 * Project:     CanWeTalkSystem 
 * By:          Salman Nouman, Minchul Hwang, Md Saiful Islam, Saje-Antoine Rose
 * Date:        April, 02, 2024
 * Description: This file contains the defined values, dependencies and function prototypes to enable the use of a user interface via the ncurses library in the chat-client
 */

#ifndef UI_H
#define UI_H

#include <ncurses.h>
#include <string.h>
#include "../../Common/inc/message.h"


// Constants here
#define DEFAULT_STYLE 0
#define HEADER_STYLE 1
#define ISTREAM_STYLE 2
#define LABELS "        IP       USER                      MESSAGE                     TIME     "
#define CLIENT_NAME "C H A T L I T E"
#define OUTGOING_LABEL "Outgoing Message"
#define MAX_MESSAGE_HISTORY 11
#define OUT_AREA_HEIGHT 6
#define STATIC_HEADER_HEIGHT 3
#define OFFSET_BOTTOM 4
#define OFFSET_TOP 3
#define ISTREAM_START 1
#define ISTREAM_WIN_HEIGHT 3
#define NEXT_COLUMN 1
#define NEXT_ROW 1

// Heading printer that is centered and colored
void refreshUI(WINDOW *staticMessagesHeader, WINDOW *staticOutgoingHeader, WINDOW *messageWindow, WINDOW *outgoingWindow);
void initializeUI(int *rows, int *cols, WINDOW **staticMessagesHeader, WINDOW **staticOutgoingHeader, WINDOW **messageWindow, WINDOW **outgoingWindow);
void printHeader(WINDOW *win, int starty, int width, const char *string, int colorScheme);
void printMessage(WINDOW* messageWindow, const Message* chatMessage, char* clientIp);

#endif
