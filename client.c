/* client.c    Kyrylo Bakumenko    29 May, 2023
 *
 * This file provides the client side for the Nuggets project in CS50.
 * The client is launched by the user providing hostname and port to connect to the server (see DESIGN.md).
 * After connecting to the server the user declares themselves with a PLAY message. From this point, responses from
 * the server are displayed using the ncurses library and the user interacts with the display using keystrokes.
 * 
 * Exit codes: 1 -> invalid number of arguments
 *             2 -> null argument passed
 *             3 -> failure to initialize messages module
 *             4 -> bad hostname or port
 *             5 -> failed screen verification
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include "message.h"

// internal function prototypes
static bool handleMessage(void* arg, const addr_t from, const char* message);
static bool handleInput(void* arg);
static void display_map(char* display);
static void initialize_curses(); // CURSES

// handleMessage helpers
static void handleOK();
static bool handleGRID(const char* message);
static bool handleGOLD(const char* message);
static void handleDISPLAY(const char* message);

// size of board, initialized to initial window size
static int NROWS;
static int NCOLS;

/* ***************************
 *  main function
 *  Accepts either 2 or 3 arguments from command-line: hostname port [playername]
 *  
 */
int main(int argc, char *argv[])
{

  /* parse the command line, validate parameters */ 
  const char* program = argv[0];
  // check num parameters
  if (argc != 3 && argc != 4) {
      fprintf(stderr, "ERROR: Expected either 2 or 3 arguments but recieved %d\n", argc-1);
      fprintf(stderr, "USAGE: %s hostname port [playername]\n", program);
      exit(1); // bad num arguments
  }
  // Defensive programming, check for null arguments
  for (int i = 1; i < argc; i++) {
      if (argv[i] == NULL) {
          fprintf(stderr, "ERROR: argmument %d is NULL\n", i);
          exit(2); // bad argument
      }
  }

  /* initialize messages module */
  // (without logging)
  if (message_init(NULL) == 0) {
      exit(3); // failure to initialize message module
  }

  /* assign command-line arguments to variables */
  // commandline provides address for server
  const char* serverHost = argv[1];
  const char* serverPort = argv[2];
  addr_t server; // address of the server
  if (!message_setAddr(serverHost, serverPort, &server)) {
      fprintf(stderr, "ERROR: Failure forming address from %s %s\n", serverHost, serverPort);
      exit(4); // bad hostname/port
  }

  // connect to server
  const char* playername = argv[3];
  if (playername != NULL) {
    char line[message_MaxBytes];
    strcpy(line, "PLAY ");
    strncat(line, playername, message_MaxBytes-strlen("PLAY "));
    message_send(server, line);
  } else {
    message_send(server, "SPECTATE");
  }

  // Loop, waiting for input or for messages; provide callback functions.
  // We use the 'arg' parameter to carry a pointer to 'server'.
  
  bool ok = message_loop(&server, 0, NULL, handleInput, handleMessage);

  // close curses
  endwin(); // CURSES
  // shut down the message module
  message_done();

  return ok ? 0 : 1;
}

/**************** handleInput ****************/
/* stdin has input ready; read a line and send it to the client.
 * Return true if the message loop should exit, otherwise false.
 * i.e., return true if EOF was encountered on stdin, or fatal error.
 */
static bool
handleInput(void* arg)
{
  // We use 'arg' to receive an addr_t referring to the 'server' correspondent.
  // Defensive checks ensure it is not NULL pointer or non-address value.
  addr_t* serverp = arg;
  if (serverp == NULL) {
    fprintf(stderr, "handleInput called with arg=NULL");
    return true;
  }
  if (!message_isAddr(*serverp)) {
    fprintf(stderr, "handleInput called without a correspondent.");
    return true;
  }

  // gather key input
  char c = getch();
  char message[strlen("KEY ") + 2];
  strcpy(message, "KEY ");
  strcat(message, &c);
  message[strlen("KEY ") + 1] = '\0';

  // send keystroke
  message_send(*serverp, message);

  // keep looping
  return false;
}

/**************** handleMessage ****************/
/* Datagram received; print it.
 * We ignore 'arg' here.
 * Return true if any fatal error.
 */
static bool
handleMessage(void* arg, const addr_t from, const char* message)
{
  // defensive programming
  if (arg == NULL || message == NULL) {
    return false;
  }

  // Find the position of the header within the message
  if (strncmp(message, "OK ", strlen("OK ")) == 0){
    handleOK();
    // CONFIRMATION??
    message_send(from, "OK");
    return false;

  } else if (strncmp(message, "GRID ", strlen("GRID ")) == 0) {
    if (!handleGRID(message)) {
      // bag grid
      message_send(from, "IGNORED");
    }
    // CONFIRMATION??
    message_send(from, "OK");
    return false;

  } else if (strncmp(message, "GOLD ", strlen("GOLD ")) == 0) {
    if (handleGOLD(message)) {
      // CONFIRMATION??
      message_send(from, "OK");
    } else {
      // CONFIRMATION??
      message_send(from, "IGNORED");
    }

  } else if (strncmp(message, "DISPLAY\n", strlen("DISPLAY\n")) == 0) {
    handleDISPLAY(message);

    // CONFIRMATION??
    message_send(from, "OK");

    return false;

  } else if (strncmp(message, "QUIT ", strlen("QUIT ")) == 0) {
    fprintf(stdout, "%s\n", message);
    return true;

  } else if (strncmp(message, "ERROR ", strlen("ERROR ")) == 0) {
    fprintf(stdout, "ERROR: From server '%s'\n", message);
    return false;

  } else {
    // MALFORMED MESSAGE -- IGNORE
    // fprintf(stderr, "ERROR: Malformed message '%s'\n", message);
    return false;
  }
  
  return false;
}

/**************** handleOK ****************/
/* no arguments                                           */
/* initializes CURSES, fills window with blank ' ' chars  */
static void
handleOK()
{
  /* initialize curses library */
  initialize_curses(); // CURSES
  display_map("");
}

/**************** handleGRID ****************/
/* takes a char* as an argument, of the GRID message type.                          */
/* GRID message must be in *exact* syntax as described in requirments.              */
/* verifies that CURSES window is at least the required size (specified by message) */
static bool
handleGRID(const char* message)
{
  int nrows;
  int ncols;
  // assign values and check for null
  if (sscanf(message, "GRID %d %d", &nrows, &ncols) != 2) {
    fprintf(stderr, "ERROR: Malformed GRID message '%s'", message);
    return false;
  }
  // if not null, verify screen size
  if (nrows > NROWS || ncols > NCOLS) {
    endwin(); // CURSES
    fprintf(stderr, "ERROR: incompatible screen of size [%d, %d] for [%d, %d]\n", NROWS, NCOLS, nrows, ncols);
    exit(5);
  } else {
    // update with screen output dimensions
    NROWS = nrows;
    NCOLS = ncols;
  }
  return true;
}

/**************** handleGOLD ****************/
/* takes a char* as an argument, of the GOLD message type.             */
/* GOLD message must be in *exact* syntax as described in requirments. */
/* displays gold values in the topline of CURSES window                */
/* if the display line is longer than the window, it is cropped.       */
static bool
handleGOLD(const char* message)
{
  // ints to store gold information to display in curse
  int gold_collected;
  int gold_purse;
  int gold_left;
  if (sscanf(message, "GOLD %d %d %d", &gold_collected, &gold_purse, &gold_left) != 3) {
    // INVALID GOLD MESSAGE SENT
    fprintf(stderr, "ERROR: Malformed GOLD message '%s'", message);
    return false;
  } else {
    // display info message
    int temp = 0; // dummy
    int ncols = 0;
    getmaxyx(stdscr, temp, ncols);
    (void)temp; // keep compiler from complaining
    for (int col = 0; col < ncols; col++) {
      move(0, col);
      addch(' ');
    }
    char buffer[ncols]; //buffer string of max length
    snprintf(buffer, ncols, "Gold Collected: %d      Gold in Purse: %d      Gold Left: %d",
                            gold_collected, gold_purse, gold_left);
    mvprintw(0,0, "%.*s", ncols, buffer);           // CURSES
    refresh();                                      // CURSES
    return true;
  }
}

/**************** handleDISPLAY ****************/
/* takes a char* as an argument, of the DISPLAY message type.             */
/* DISPLAY message must be in *exact* syntax as described in requirments. */
/* displays the chars following '\n' character in CURSES.                 */
static void
handleDISPLAY(const char* message)
{
  /* get display contents */
  size_t headerLength = strlen("DISPLAY\n");
  size_t displayLength = strlen(message) - headerLength + 1;
  // display buffer
  char display[displayLength]; 
  strncpy(display, message + headerLength, displayLength);
  display[displayLength] = '\0'; // in-case length is such that last terminating character is not copied
  
  // print display
  display_map(display);
}

/* ************ initialize_curses *********************** */
/* initialize curses // CURSES everywhere in this function */
static void
initialize_curses()
{
  // initialize the screen - which defines LINES and COLS
  initscr();

  // cache the size of the window in our global variables
  // (this is a macro, which is why we don't need & before the variables.)
  getmaxyx(stdscr, NROWS, NCOLS);

  cbreak(); // actually, this is the default
  noecho(); // don't show the characters users type

  // I like yellow on a black background
  start_color();
  init_pair(1, COLOR_YELLOW, COLOR_BLACK);
  attron(COLOR_PAIR(1));
  fprintf(stdout, "INITIALIZED CURSES\n");
  fflush(stdout);
}

/* ************ display_map *********************** */
/* Display the map (char* display) into CURSES screen */
static void
display_map(char* display)
{
  // clear output
  int max_nrows = 0;
  int max_ncols = 0;
  getmaxyx(stdscr, max_nrows, max_ncols);
  for (int row = 1; row < max_nrows; row++) {
    for (int col = 0; col < max_ncols; col++) {
      move(row, col);
      addch(' ');                               // fill with blank
    }
  }
  for (int row = 0; row < NROWS; row++) {
    for (int col = 0; col < NCOLS; col++) {
      move(row+1,col);                          // CURSES, +1 account for info line
      int idx = row * NCOLS + col;
      if (idx < strlen(display)) {                
        addch(display[row * NCOLS + col]);      // CURSES
      } else {
        addch(' ');                             // fill with blank
      }
    }
  }
  refresh();                                    // CURSES
}