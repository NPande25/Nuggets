/*
* server.c     May 22, 2023
*
* This file contains the code for the server, which handles all processes for
* the Nuggets game. It receives messages from client and calls the appropriate functions
* to adjust the game map, players, and more. When the game is over (all the gold is collected),
* the server sends all clients a game summary.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "file.h"
#include "message.h"
#include "mem.h"
#include "player.h"
#include "grid.h"
#include "gridcell.h"

/**************** file-local functions ****************/

static bool handleMessage(void* arg, const addr_t from, const char* message);
static void parseArgs(const int argc, char* argv[], char** mapFileName, int* seed);
static void addPlayer(addr_t from, const char* name);
static void addSpectator(addr_t from);
static void handleKey(player_t* player, const char* key);
static bool moveOnMap(player_t* player, int newX, int newY);
static void handleQuit(player_t* player); 
static void dropGold();
static void updatePlayers();
static void updateSpectator();
static void gameOver();

struct gameData {
    grid_t* map;
    player_t* allPlayers[26];
    addr_t spect;
    bool hasSpect;
    int numPlayers;
    int numGold;
    int numRows;
    int numCols;
    addr_t justFoundGold;
};

static struct gameData game; //global variable for game data

/***************** main *******************************/
int 
main (const int argc, char* argv[])
{
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

    //initalize grid using map file and array of players

    grid_t* gameMap = grid_new(numCol, numRow); //get row and column size
    grid_load(gameMap, mapFileName);
    game.map = gameMap;
    //game.allPlayers = mem_calloc(26, sizeof(player_t));
    //game.spect = NULL;
    game.hasSpect = false;
    game.numPlayers = 0;
    game.numGold = 250;
    game.numRows = numRow;
    game.numCols = numCol;
    dropGold();

    // initialize the message module (without logging)
    int myPort = message_init(NULL);
    if (myPort == 0) {
        return 2; // failure to initialize message module
    } else {
        printf("serverPort=%d\n", myPort);
    }

    // Loop, waiting for input or for messages; provide callback functions.
    bool ok = message_loop(NULL, 0, NULL, NULL, handleMessage);

    // shut down the message module
    message_done();

    // clear memory for grid and players
    grid_delete(gameMap);
    for(int i = 0; i<game.numPlayers; i++){
        player_delete(game.allPlayers[i]);
    }
    //mem_free(game);

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
/* Datagram received; print it, parse it. and call appropriate methods.
* Send "malformed message" if message is invalid
* We ignore 'arg' here.
*/
static bool
handleMessage(void* arg, const addr_t from, const char* message)
{    
    // print the message and a prompt
    printf("'%s'\n", message);
    printf("> ");
    fflush(stdout);

    if (strncmp(message, "PLAY ", strlen("PLAY ")) == 0) {
        const char* playerName = message + strlen("PLAY ");
        addPlayer(from, playerName);
        printf("PLAY: %s\n", playerName);
    } 
    else if (strncmp(message, "SPECTATE", strlen("SPECTATE")) == 0) {
        addSpectator(from);
        printf("SPECTATE\n");
    }
    else if (strncmp(message, "KEY ", strlen("KEY ")) == 0) {
        const char* keystroke = message + strlen("KEY ");
        printf("KEY: %s\n", keystroke);

        if(!message_eqAddr(from, game.spect)) {
            //get moving player
            player_t* mover = NULL;
            for(int i = 0; i<game.numPlayers; i++) {
                if (message_eqAddr(from, player_get_addr(game.allPlayers[i]))) {
                    mover = game.allPlayers[i];
                }
            }

            //move player on master grid
            handleKey(mover, keystroke);

            //update player visibility
            player_playerVisibility(mover, game.map);
        }
        else {
            //spectator can only quit
            if (strcmp(keystroke, "Q") == 0) {
                player_t* playerSpect = player_new('.', "SPECTATOR", from, game.numRows, game.numCols);
                handleQuit(playerSpect);
                player_delete(playerSpect);
            }
        }
    }
    else {
        fprintf(stderr, "ERROR: malformed message");
        message_send(from, "ERROR malformed message\n");
    }

    updatePlayers();
    updateSpectator();

    if (game.numGold == 0) {
        gameOver();
        return true;
    }

    return false;
}

/**************** addPlayer ****************/
/* Recieves an address and player name
* Create a new player using the address, name and character based on number of players.
* Adds new player to the game's player array
* Drops player in random spot in the map
*/

static void
addPlayer(addr_t from, const char* name)
{
    printf("name: %s\n", name);

    const int maxPlayers = 26;

    if (game.numPlayers == maxPlayers-1) {
        message_send(from, "QUIT Game is full: no more players can join.");
    }
    else if (name == NULL) {
        message_send(from, "QUIT Sorry - you must provide player's name.");
    }
    else { //create new player and add to array of players
        
        //truncate name and replace invalid characters with '_'
        char newName[60];
        sprintf(newName, "%.50s", name); //max name length is 50 characters
        int length = strlen(newName);
        for (int i = 0; i < length; i++) {
            if (!isgraph(newName[i]) && !isblank(newName[i])) {
                newName[i] = '_';
            }
        }

        //get player letter
        int curNumPlayers = game.numPlayers;
        char playerLetter = 'A' + curNumPlayers;

        //create new player
        player_t* newPlayer = player_new(playerLetter, newName, from, game.numRows, game.numCols);
        
        if (newPlayer != NULL) {
            game.allPlayers[game.numPlayers] = newPlayer;   
            game.numPlayers++;

            //send OK message to client
            char okMsg[10];
            sprintf(okMsg, "OK %c\n", playerLetter);
            message_send(from, okMsg);  

            //send GRID message to client   
            char gridMsg[100];
            sprintf(gridMsg, "GRID %d %d\n", game.numRows, game.numCols);
            message_send(from, gridMsg);

            //drop player in randomly selected room spot in map
            bool dropped = false;
            while (!dropped) {
                int x = rand() % (game.numCols);
                int y = rand() % (game.numRows);

                if(gridcell_getC(grid_get(game.map, x, y)) == '.') {
                    grid_set(game.map, x, y, playerLetter);
                    player_set_x(newPlayer, x);
                    player_set_y(newPlayer, y);
                    dropped = true;
                }
            }

            //update player visibility
            player_playerVisibility(newPlayer, game.map);
        }
    }
}


/**************** addSpectator ****************/
/* Recieves an address for spectator client
* Checks if there is an existing spectator. If there is, sends QUIT message to existing one
* and replaces it.
*/
static void
addSpectator(addr_t from)
{
    //if there is already a spectator, send QUIT
    if (game.hasSpect == true) {
        message_send(game.spect, "QUIT You have been replaced by a new spectator");
    }
    
    //create new spectator
    game.spect = from;
    game.hasSpect = true;

    //send GRID message to client   
    char gridMsg[100];
    sprintf(gridMsg, "GRID %d %d\n", game.numRows, game.numCols);
    message_send(from, gridMsg);
}

/**************** handleKey ****************/
/* Receives player and keystroke
* Uses switch cases to call moveOnMap method with appropriate parameters depending on
* where player is trying to move.
*/
static void 
handleKey(player_t* player, const char* key) 
{
    switch (*key) {
    case 'h': 
        moveOnMap(player, player_get_x(player)-1, player_get_y(player));
        break;
    case 'l': 
        moveOnMap(player, player_get_x(player)+1, player_get_y(player));
        break;
    case 'j': 
        moveOnMap(player, player_get_x(player), player_get_y(player)+1);
        break;
    case 'k': 
        moveOnMap(player, player_get_x(player), player_get_y(player)-1);
        break;
    case 'y': 
        moveOnMap(player, player_get_x(player)-1, player_get_y(player)-1);
        break;
    case 'u': 
        moveOnMap(player, player_get_x(player)+1, player_get_y(player)-1);
        break;
    case 'b': 
        moveOnMap(player, player_get_x(player)-1, player_get_y(player)+1);
        break;
    case 'n': 
        moveOnMap(player, player_get_x(player)+1, player_get_y(player)+1);
        break;
     case 'H': 
        while(moveOnMap(player, player_get_x(player)-1, player_get_y(player))){
            player_playerVisibility(player, game.map);
        }
        break;
    case 'L': 
        while(moveOnMap(player, player_get_x(player)+1, player_get_y(player))){
            player_playerVisibility(player, game.map);
        }
        break;
    case 'J': 
        while(moveOnMap(player, player_get_x(player), player_get_y(player)+1)){
            player_playerVisibility(player, game.map);
        }
        break;
    case 'K': 
        while(moveOnMap(player, player_get_x(player), player_get_y(player)-1)){
            player_playerVisibility(player, game.map);
        }
        break;
    case 'Y': 
        while(moveOnMap(player, player_get_x(player)-1, player_get_y(player)-1)){
            player_playerVisibility(player, game.map);
        }
        break;
    case 'U': 
        while(moveOnMap(player, player_get_x(player)+1, player_get_y(player)-1)){
            player_playerVisibility(player, game.map);
        }
        break;
    case 'B': 
        while(moveOnMap(player, player_get_x(player)-1, player_get_y(player)+1)){
            player_playerVisibility(player, game.map);
        }
        break;
    case 'N': 
        while(moveOnMap(player, player_get_x(player)+1, player_get_y(player)+1)){
            player_playerVisibility(player, game.map);
        }
        break;
    case 'Q':
        handleQuit(player);
        break;
    default:
        fprintf(stderr, "ERROR usage: unknown keystroke");
        message_send(player_get_addr(player), "ERROR usage: unknown keystroke");
        break;
    }
}

/**************** moveOnMap ****************/
/* Recieves player, and x/y coordinates of location it is trying to mnove to
* Checks if there is an open spot, another player, or gold at the location the player is
* trying to move it, and adjusts the master grid accordingly.
* Returns true if move is successful, false if not.
*/
static bool
moveOnMap(player_t* player, int newX, int newY)
{
    if (player_is_active(player) == true) {
        int curX = player_get_x(player);
        int curY = player_get_y(player);
        gridcell_t* curCell = grid_get(game.map, curX, curY);
        gridcell_t* newCell = grid_get(game.map, newX, newY);
        char curChar = gridcell_getC(curCell);
        if(newCell == NULL || curCell == NULL) {
            return false;
        }
        else {
            char newChar = gridcell_getC(newCell);
            if(isupper(newChar)) { 
                int checkLetter = newChar - 'A';
                player_t* otherPlayer = game.allPlayers[checkLetter];
                grid_set(game.map, curX, curY, newChar); 
                player_set_x(otherPlayer, curX);
                player_set_y(otherPlayer, curY);

                grid_set(game.map, newX, newY, curChar); 
                player_set_x(player, newX);
                player_set_y(player, newY);

                return true;
            }
            else if (newChar == '*') {
                int pileGold = gridcell_getGold(newCell);
                int newScore = player_get_score(player) + pileGold;
                player_set_score(player, newScore);
                game.numGold -= pileGold;
                grid_set(game.map, newX, newY, curChar); 
                player_set_x(player, newX);
                player_set_y(player, newY);
                if(gridcell_getRoom(curCell)) {
                    grid_set(game.map, curX, curY, '.');
                }
                else {
                    grid_set(game.map, curX, curY, '#');
                }

                //send GOLD message to player
                char goldMsg[100];
                sprintf(goldMsg, "GOLD %d %d %d\n", pileGold, player_get_score(player), game.numGold);
                message_send(player_get_addr(player), goldMsg);
                game.justFoundGold = player_get_addr(player);

                return true;
            }
            else if (newChar == '.' || newChar == '#') {
                grid_set(game.map, newX, newY, curChar);
                player_set_x(player, newX);
                player_set_y(player, newY);

                if(gridcell_getRoom(curCell)) {
                    grid_set(game.map, curX, curY, '.');
                }
                else {
                    grid_set(game.map, curX, curY, '#');
                }

                return true;
            }
        }
    }

    return false;
}


/**************** handleQuit ****************/
/* Recieves a player that sent quit command
* If the player is the spectator, change the game's hasSpect boolean to false.
* If the player is a normal player, deactivate them. In an case, send the player's 
* client a QUIT message.
*/
static void
handleQuit(player_t* player) 
{
    if(message_eqAddr(player_get_addr(player), game.spect)) {
        game.hasSpect = false;
        message_send(game.spect, "QUIT Thanks for watching!");
    }
    else {
        //remove player's symbol from map
        int curX = player_get_x(player);
        int curY = player_get_y(player);
        gridcell_t* curCell = grid_get(game.map, curX, curY);
        if(gridcell_getRoom(curCell)) {
            grid_set(game.map, curX, curY, '.');
        }
        else {
            grid_set(game.map, curX, curY, '#');
        }

        player_deactivate(player);
        message_send(player_get_addr(player), "QUIT Thanks for playing!");
    }
}


/**************** dropGold ****************/
/* Drops gold at random locations of the grid. Drops a random number of piles
* between the minimum and maximum number of piles. The numnber of gold in all the piles
* sums up to 250.
*/
static void
dropGold()
{
    const int goldTotal = 250; //amount of gold in game
    const int goldMinNumPiles = 10; //minimum number of gold piles
    const int goldMaxNumPiles = 30; //maximum number of gold piles

    int numPiles = (rand() % (goldMaxNumPiles - goldMinNumPiles)) + goldMinNumPiles;
    
    int remaining = goldTotal; //remaining gold to drop
    int bound = (int) (goldTotal / numPiles);
    int numGoldInPile = 0;

    for (int i = 0; i<numPiles-1; i++) {
        numGoldInPile = (rand() % (bound-1)) + 1;

        //drop gold
        bool dropped = false;
        while (!dropped) {
            int x = rand() % (game.numCols);
            int y = rand() % (game.numRows);

            char randCell = gridcell_getC(grid_get(game.map, x, y));
            if(randCell== '.') {
                grid_set(game.map, x, y, '*');
                gridcell_setGold(grid_get(game.map, x, y), numGoldInPile);
                dropped = true;
            }
        }
        remaining = remaining - numGoldInPile;
    }

    bool dropped = false;
    while (!dropped) {
        int x = rand() % (game.numCols);
        int y = rand() % (game.numRows);

        if(gridcell_getC(grid_get(game.map, x, y)) == '.') {
            grid_set(game.map, x, y, '*');
            gridcell_setGold(grid_get(game.map, x, y), remaining);            
            dropped = true;
        }
    }
}


/**************** updatePlayers ****************/
/* Loops through players and sends GOLD and DISPLAY messages to their clients.
*/
static void
updatePlayers()
{
     for(int i = 0; i<game.numPlayers; i++) {
        //get current player
        player_t* curPlayer = game.allPlayers[i];
        if(curPlayer == NULL){
            printf("player is null\n");
        }

        if (!message_eqAddr(game.justFoundGold, player_get_addr(curPlayer))) {
            //send GOLD message to players
            char goldMsg[100];
            int score = player_get_score(curPlayer);
            sprintf(goldMsg, "GOLD %d %d %d\n", 0, score, game.numGold);
            message_send(player_get_addr(curPlayer), goldMsg);
        }
        
        //send DISPLAY message to players
        char* gridString = player_get_string(curPlayer, game.map);
        char* displayMsg = mem_malloc((sizeof(char) * strlen(gridString)) + 10);
        strcpy(displayMsg, "DISPLAY\n");
        strcat(displayMsg, gridString);
        message_send(player_get_addr(curPlayer), displayMsg);
        mem_free(gridString);
        mem_free(displayMsg);
    }
}


/**************** updateSpectator ****************/
/* Sends GOLD and DISPLAY message to the spectator.
*/
static void
updateSpectator()
{
    if (game.hasSpect) {
        //send GOLD message to specator
        char goldMsg[100];
        sprintf(goldMsg, "GOLD %d %d %d\n", 0, 0, game.numGold);
        message_send(game.spect, goldMsg);

        //send DISPLAY message to spectator
        grid_update_map(game.map);
        char* gridString = grid_get_map(game.map);
        char* displayMsg = mem_malloc((sizeof(char) * strlen(gridString)) + 9);
        strcpy(displayMsg, "DISPLAY\n");
        strcat(displayMsg, gridString);
        message_send(game.spect, displayMsg);
        //mem_free(gridString);
        mem_free(displayMsg);
    }
}


/**************** gameOver ****************/
/* Creates game over message with player data and sends it to the 
* client of every player.
*/
static void
gameOver() 
{
    //get game over message
    char gameOverMsg[100];
    sprintf(gameOverMsg, "QUIT GAME OVER:\n");
    for (int i = 0; i<game.numPlayers; i++) {
        player_t* curPlayer = game.allPlayers[i];
        char playerData[50];
        sprintf(playerData, "%-3c %7d %s\n", player_get_c(curPlayer), player_get_score(curPlayer), player_get_name(curPlayer));
        strcat(gameOverMsg, playerData);
    }

    //print summary
    printf("%s", gameOverMsg);

    //send game over message to all players
    for (int i = 0; i<game.numPlayers; i++) {
        message_send(player_get_addr(game.allPlayers[i]), gameOverMsg);
    }

    //send game over message to spectator
    if (game.hasSpect) {
        message_send(game.spect, gameOverMsg);
    }
}
