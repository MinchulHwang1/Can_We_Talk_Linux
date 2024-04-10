/*
 * Filename:    ui.c
 * Project:     CanWeTalkSystem 
 * By:          Salman Nouman, Minchul Hwang, Md Saiful Islam, Saje-Antoine Rose
 * Date:        April, 02, 2024
 * Description: This file contains functionality to handle the ui opertations via the use of the ncurses library.
 */

#include "../inc/ui.h"

/*
 * Function:    printHeader
 * Description: This function displayes a centered header text in an ncurses window
 * Parameters:  WINDOW *win: A pointer to an ncurses window where the header will be printed.
 *              int yBegin: The starting y-coordinate for the header.
 *              int width: The width of the window
 *              const char *string: The header text in the window to be printed
 *              int colorScheme: The colour pair to use for the header
 * Returns:     void
 */
void printHeader(WINDOW *win, int yBegin, int width, const char *string, int colorScheme) 
{
	// Centering the heading text here
    int length;
	int x;
	int y;
    float temp;

    if (win == NULL) 
	{
        win = stdscr;
    }
    getyx(win, y, x);
    if (yBegin != 0) 
	{
        y = yBegin;
    }
    length = strlen(string);
    temp = (width - length) / 2;// This ensures the cursor is at the middle of the text
    x = (int)temp;

    wbkgdset(win, COLOR_PAIR(colorScheme) | ' ');
    mvwhline(win, yBegin - 1, 0, ' ', width); // Color the line above
    mvwhline(win, yBegin, 0, ' ', width);     // Color the line of the heading
    mvwhline(win, yBegin + 1, 0, ' ', width); // Color the line below

    wattron(win, COLOR_PAIR(colorScheme));
    mvwprintw(win, y, x, "%s", string);// Setting the attribute for the header
    wattroff(win, COLOR_PAIR(colorScheme));

    refresh(); // Refresh the window to show the string
}


/*
 * Function:    printMessage
 * Description: This function displayes a centered header text in an ncurses window
 * Parameters:  WINDOW *messageWindow: A pointer to an ncurses window where the header will be printed.
 *              const Message* chatMessage: A pointer to a Message structure containing information about the chat message to be displayed
 *              char* clientIp: The buffer to contain the ip address
 * Returns:     void
 */
void printMessage(WINDOW* messageWindow, const Message* chatMessage, char* clientIp) 
{
    if (messageWindow == NULL) 
	{
        return;
    }

    int x, y;
    getyx(messageWindow, y, x); // Get the current cursor position
	wmove(messageWindow, y, x);
	if (strcmp(chatMessage->ip, clientIp) == 0)
	{
		wprintw(messageWindow, "%-15s [%-5s] >> %-40s (%s)\n", chatMessage->ip, chatMessage->userName, 
		chatMessage->message, chatMessage->timeStamp);
	}
	else
	{
		wprintw(messageWindow, "%-15s [%-5s] << %-40s (%s)\n", chatMessage->ip, chatMessage->userName, 
		chatMessage->message, chatMessage->timeStamp);
	}

    wrefresh(messageWindow);
}


/*
 * Function:    refreshUI
 * Description: This function updates the values displayed on the UI
 * Parameters:  WINDOW *staticMessagesHeader: A pointer to an ncurses window for displaying static messages.
 *              WINDOW *staticOutgoingHeader: A pointer to an ncurses window for displaying static outgoing messages.
 *              WINDOW *messageWindow: A pointer to an ncurses window where the main chat messages are.
 *              WINDOW *outgoingWindow: A pointer to an ncurses window for displaying outgoing messages (user input)
 * Returns:     void
 */
void refreshUI(WINDOW *staticMessagesHeader, WINDOW *staticOutgoingHeader, WINDOW *messageWindow, WINDOW *outgoingWindow) 
{
    printHeader(staticMessagesHeader, NEXT_ROW, getmaxx(staticMessagesHeader), CLIENT_NAME, HEADER_STYLE);
    mvwprintw(staticMessagesHeader, OFFSET_BOTTOM, 0, "%s", LABELS);
    printHeader(staticOutgoingHeader, NEXT_ROW, getmaxx(staticOutgoingHeader), OUTGOING_LABEL, HEADER_STYLE);

    wattron(outgoingWindow, COLOR_PAIR(ISTREAM_STYLE));
    mvwaddch(outgoingWindow, ISTREAM_START, 0, '>');
    wattroff(outgoingWindow, COLOR_PAIR(ISTREAM_STYLE));

    wrefresh(staticOutgoingHeader);
    wrefresh(staticMessagesHeader);
    wrefresh(messageWindow);
    wrefresh(outgoingWindow);
}

/*
 * Function:    initializeUI
 * Description: Initializes the user interface (UI) for the chat application using ncurses.
 * Parameters:  int *rows: Pointer to store the number of rows in the terminal.
 *              int *cols: Pointer to store the number of columns in the terminal.
 *              WINDOW **staticMessagesHeader: Pointer to a window for displaying static messages header.
 *              WINDOW **staticOutgoingHeader: Pointer to a window for displaying static outgoing messages header.
 *              WINDOW **messageWindow: Pointer to a window where the main chat messages are displayed.
 *              WINDOW **outgoingWindow: Pointer to a window for displaying outgoing messages (user input).
 * Returns:     void
 */
void initializeUI(int *rows, int *cols, WINDOW **staticMessagesHeader, WINDOW **staticOutgoingHeader, WINDOW **messageWindow, WINDOW **outgoingWindow) 
{
    initscr();
    cbreak();
    noecho();
    start_color();
    init_pair(DEFAULT_STYLE, COLOR_WHITE, COLOR_BLACK);
    init_pair(HEADER_STYLE, COLOR_BLACK, COLOR_WHITE);
    init_pair(ISTREAM_STYLE, COLOR_YELLOW, COLOR_BLACK);

    getmaxyx(stdscr, *rows, *cols);

    *staticMessagesHeader = newwin(STATIC_HEADER_HEIGHT + STATIC_HEADER_HEIGHT, *cols, 0, 0);
    box(*staticMessagesHeader, 0, 0);

    *staticOutgoingHeader = newwin(STATIC_HEADER_HEIGHT, *cols, *rows - OUT_AREA_HEIGHT, 0);
    box(*staticOutgoingHeader, 0, 0);

    *messageWindow = newwin(MAX_MESSAGE_HISTORY, *cols, STATIC_HEADER_HEIGHT + OFFSET_TOP, 0);
    scrollok(*messageWindow, TRUE);

    *outgoingWindow = newwin(ISTREAM_WIN_HEIGHT, *cols, *rows - ISTREAM_WIN_HEIGHT, 0);

    refreshUI(*staticMessagesHeader, *staticOutgoingHeader, *messageWindow, *outgoingWindow);
}