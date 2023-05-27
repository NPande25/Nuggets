/*
* server.c     May 22, 2023
*
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "file.h"
#include "message.h"
//#include "mem.h"
// #include "player.h"
// #include "grid.h"
// #include "gridcell.h"

/**************** file-local functions ****************/

static bool handleMessage(void* arg, const addr_t from, const char* message);
static void parseArgs(const int argc, char* argv[], char** mapFileName, int* seed);
//static void addPlayer(gameData_t* game, addr_t from, char* name);
//static void addSpectator(gameData_t* game, addr_t from);


struct gameData {
//    grid_t* map;
 //   player_t** allPlayers;
    player_t* spect;
    bool hasSpect;
    int numPlayers;
    int numGold;
};

/***************** main *******************************/
int 
main (const int argc, char* argv[])
{
    //list of game parameters
  /*  const int maxNameLength = 50; //max number of chars in player's name
    const int maxPlayers = 26; // max number of players
    const int goldTotal = 250; //amount of gold in game
    const int goldMinNumPiles = 10; //minimum number of gold piles
    const int goldMaxNumPiles = 30; //maximum number of gold piles
*/
    // parse arguments
    char* mapFileName = NULL;
    int seed = 0;
    parseArgs(argc, argv, &mapFileName, &seed);

    if (seed == -1) {
        srand(getpid());
    }
    else {
        srand(seed);
    }

    printf("%s %d\n", mapFileName, seed);

    // get number of rows and columns in map
    FILE* fp = fopen(mapFileName, "r");
    int numCol = 0; // number of columns
    bool addCol = true;
    int numRow = 0; // number of rows
    for (char c = getc(fp); c != EOF; c = getc(fp)) {
        if (c == '\n') { // increment row count if character is newline
            numRow = numRow + 1;
            addCol = false;
        }
        if (addCol) { // increment columns if still on first row
            numCol = numCol + 1;
        }
    }
    printf("%d %d\n", numCol, numRow);
    fclose(fp);

    // initalize grid using map file and array of players

   // grid_t* gameMap = grid_new(); //get row and column size
   // grid_load(gameMap, mapFileName);
   // player_t** players = 
   // struct gameData* game = (struct gameData*)mem_malloc(sizeof(struct gameData));
   // game->map = gameMap
   // game->allPlayers = players
   // game->numPlayers = 0

    // initialize the message module (without logging)
    int myPort = message_init(NULL);
    if (myPort == 0) {
        return 2; // failure to initialize message module
    } else {
        printf("serverPort=%d\n", myPort);
    }

    // Loop, waiting for input or for messages; provide callback functions.
    // We use the 'arg' parameter to carry a pointer to 'server'.
    bool ok = message_loop(NULL, 0, NULL, NULL, handleMessage);

    // shut down the message module
    message_done();

    // clear memory for grid
    //grid_delete(gameMap);
   // mem_free(game);

    return ok? 0 : 1; // status code depends on result of message_loop
}

//**************** parseArgs ****************/
/* Receive command line inputs, checks if inputs suit usage and are valid  
* Stores inputs in variables if valid. Does not return anything.
*/
static 
void parseArgs(const int argc, char* argv[], char** mapFileName, int* seed)
{
    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Error: wrong number of arguments. Must only one or two arguments: map file name and seed(optional).\n");
        exit(1);
    }

    // check if map file is valid
    *mapFileName = argv[1];
    FILE* fp = fopen(*mapFileName, "r");
    if (fp != NULL) { 
        fclose(fp);
    }
    else {
        fprintf(stderr, "Error: mapFileName is invalid.\n");
        exit(2);
    }

    // check if seed is valid
    if (argc == 3) {
        char* seedInput = argv[2];

        //convert seed to int using scanf
        char nextchar;
        if (sscanf(seedInput, "%d%c", seed, &nextchar) != 1) {
            fprintf(stderr, "seed is not an a valid integer.\n");
            exit(3);
        }
    }
    else {
        *seed = -1;
    }
}

/**************** handleMessage ****************/
/* Datagram received; print it, read a line from stdin, and use it as reply.
* We ignore 'arg' here.
* Return true if EOF on stdin or any fatal error.
*/
static bool
handleMessage(void* arg, const addr_t from, const char* message)
{
   // struct gameData* curData = arg;
    
    // print the message and a prompt
    printf("'%s'\n", message);
    printf("> ");
    fflush(stdout);

    if (strncmp(message, "PLAY ", strlen("PLAY ")) == 0) {
        const char* playerName = message + strlen("PLAY ");
        //addPlayer(curData, from, playerName);
        printf("PLAY: %s\n", playerName);
    } 
    else if (strncmp(message, "SPECTATE", strlen("SPECTATE")) == 0) {
        //addSpectator(curData, from);
        printf("SPECTATE\n");
    }
    else if (strncmp(message, "KEY ", strlen("KEY ")) == 0) {
        const char* keystroke = message + strlen("KEY ");
        printf("KEY: %s\n", keystroke);
        //handleKey(from, keystroke);
    }
    else {
        message_send(from, "invalid message\n");
    }
    return false;
/*
    // allocate a buffer into which we can read a line of input
    // (it can't be any longer than a message)!
    char line[message_MaxBytes];

    // read a line from stdin: this is VERY unusual for a handleMessage()
    if (fgets(line, message_MaxBytes, stdin) == NULL) {
        // EOF case: stop looping
        putchar('\n');
        return true;
    } else {
        // strip trailing newline
        const int len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        */
/*
        if (strncmp(message, "PLAY ", strlen("PLAY ")) == 0) {
            const char* content = message + strlen("PLAY ");

            if (curData->numPlayers == 26) {
                message_send(from, "QUIT Game is full: no more players can join.");
            }
        } else {

        }
        // send as message back to client
        message_send(from, line);

        // normal case: keep looping
        return false;
    }
    */
}

static void
addPlayer(gameData_t* game, addr_t from, char* name)
{
    const int maxPlayers = 26;
    if (game->numPlayers == maxPlayers) {
        message_send(from, "QUIT Game is full: no more players can join.");
    }
    else { //create new player and add to array of players

    }

    //drop player in randomly selected room spot in map

}

static void
addSpectator(gameData_t* game, addr_t from)
{
    //if there is already a spectator, send QUIT
    if (game->hasSpect == true) {
        message_send(spectator_getAddress(game->spect), "QUIT New spectator has arrived");
    }
    
    //create new spectator
    player_t* newSpect = spectator_new(from);
    game->spect = newSpect;

    //drop player in randomly selected room spot in map

}

static void 
handleKey(gameData_t* game, addr_t from, char* key) 
{
    switch (key) {
    case 'h': 
        break;
    case 'l': 
        break;
    case 'j': 
        break;
    case 'k': 
        break;
    case 'y': 
        break;
    case 'u': 
        break;
    case 'b': 
        break;
    case 'n': 
        break;
    case 'Q':
        break;
    default:
        break;
    }
}

static void
dropGold(gameData_t* game, const int goldTotal, const int goldMinNumPiles, const int goldMaxNumPiles)
{


}
*/
