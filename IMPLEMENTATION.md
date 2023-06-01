# CS50 Nuggets
## Implementation Spec
### CecsC (Team 17), Spring, 2023


According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our implementation also includes `player`, `grid`, and `gridcell` modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.


> **Extra Credit:**
The additional functionality is as follows:


As a non-spectator player of the game, if you "step" on top of another player's character, you steal 10% of their gold that they have, none if they have no gold. It also displays a message to both the theif and the victim, detailing how much gold was stolen, and their remaining total gold.


The modules that are affected are the `server` module, and the `client` module.


Functions affected:


The `static bool moveOnMap(player_t* player, int newX, int newY)` function within the `server` module has added logic that checks whether another player has been stepped on, and if so, calculates 10% of the other player's gold and transfers it to the player doing the motion.


Pseudocode:


#### `moveOnMap`:
```
if player is active
   get player coordinates
   get gridcell player is in (curCell) and gridcell of cell they are trying to move to (newCell)
   get the player's character
   if both cells are not null
       get char of newCell(newChar)
       if newChar is an uppercase letter
           swap the letters in newCell and curCell
           give 10% of gold from player stepped on to mover
           return true
       else if newChar is '*'
           get amount of gold in pile
           add gold amount to player's score
           subtract gold in pile from game's remaining gold
           move player to new position
           change char in old position to '.' or '*'
           send GOLD message to player
           return true
       else if newChar is '.' or '*'
           move player to new position
           change char in old position to '.' or '*'
           return true
return false
```

> ---




## Plan for division of labor


We have 4 group members, Nikhil, Andy, Kyrylo, and Logan.


Nikhil is responsible for the grid and gridcell modules, focusing on the visibility aspect of the game. Logan is responsible for the player module, and to assist Nikhil with the grid module. Kyrylo will be responsible for the client, and Andy will be in charge of designing the server.


Nikhil, Andy, and Kyrylo will be testing while Logan works on the support and documentation.


## Player


### Data structures


bool* boolGrid
boolGrid is a 1d array of booleans aligning with the main map, where each boolean is true if the corresponding character on the map has been seen by the player, false otherwise


struct grid_t
Grid_t contains a 1d array of gridcells, each representing a character on the map that the players move on.


### Definition of function prototypes


The `player` module implements several functions.


`player_new` creates a new player


`player_playerVisibility` updates a player's visibility grid (boolGrid) based on the main grid after each player moves.


`player_get_string` takes the current grid and print out the string map that the specific player sees themselves.


`player_delete` deletes a `player_t` struct.


Then, the module implements a series of getters and setters for various members of the `player_t` struct, which are:
* `player_get_addr`
* `player_get_boolGrid`
* `player_get_c`
* `player_get_name`
* `player_get_score`
* `player_get_x`
* `player_get_y`
* `player_is_active`
* `player_set_c`
* `player_set_name`
* `player_set_score`
* `player_set_boolGrid`
* `player_set_x`
* `player_set_y`
* `player_deactivate`




Function prototypes:
```c
player_t* player_new(char c, const char* name, const addr_t addr, int NR, int NC);
void player_delete(player_t* player);
void player_playerVisibility(player_t* player, grid_t* grid);
addr_t player_get_addr(player_t* player);
bool player_get_boolGrid(player_t* player, int index);
char player_get_c(player_t* player);
const char* player_get_name(player_t* player);
int player_get_score(player_t* player);
int player_get_x(player_t* player);
int player_get_y(player_t* player);
bool player_is_active(player_t* player);
void player_set_c(player_t* player, char c);
void player_set_name(player_t* player, char* name);
void player_set_score(player_t* player, int score);
void player_set_boolGrid(player_t* player, int index, bool visible);
void player_set_x(player_t* player, int x);
void player_set_y(player_t* player, int y);
void player_deactivate(player_t* player);
char* player_get_string(player_t* player, grid_t* grid);
```


### Detailed pseudo code


#### `player_new`
```
allocate for player
allocate for boolGrid
for each index in boolGrid
   set to false
initialize each member of player
return the player
```


#### `player_playerVisibility`:
```
for each gridcell in the main map grid
   checks if each cell is visable from the player's current position
   if yes, mark that index as true
   if not, mark as false
```


#### `player_delete`
```
free the boolGrid
free the player struct
```


#### `player_get_string`:
```
for each gridcell in the main map grid
   if the gridcell is visible to player
       if it is gold
           if the player can currently seen it
               show the gold
           otherwise show it as an empty room space
       else if it is the player's character
           show it as @ symbol
       otherwise, print the gridcell as it comes
```


## Grid




### Definition of function prototypes


`grid` implements several functions in `grid.c`:


`grid_new`, creates a new `grid_t` struct.


`grid_get_NR` returns the number of rows.


`grid_get_NC` returns the number of columns.


`grid_get_gridarray` returns the gridcell in the gridarray at a specified index.


`grid_get_map` returns the map, a char* string in the grid that holds the map.


`grid_load` loads a grid from a file specified by the path name. The file represents a grid where each character in the file corresponds to a cell in the grid.


`grid_update_map` updates the map string after it has changed, when a player has moved


`grid_set` changes the character at a certain location in the grid.
`grid_print` prints the char* map of the grid. This is mostly used for testing/debugging purposes. For the game, we have more sophisticated `get_string` methods.


`grid_iterate` iterates over all the gridcells in the grid.


`grid_isVisible` is the base visibility function for the game. It determines whether a target gridcell is visible from a player gridcell.


`grid_generateGold` creates a random number of gold piles in a grid, between minPiles and maxPiles.


`grid_delete` deletes the `grid_t` struct.


```c
grid_t* grid_new();
void grid_load(grid_t* grid, char* pathName);
int grid_get_NR(grid_t* grid);
int grid_get_NC(grid_t* grid);
gridcell_t* grid_get_gridarray(grid_t* grid, int idx);
char* grid_get_map(grid_t* grid);
void grid_set(grid_t* grid, int x, int y, char c);
void grid_print(grid_t* grid);
void grid_update_map(grid_t* grid);
void grid_iterate(grid_t* grid, void* arg, void (*itemfunc)(void* arg, void* item));
bool grid_isVisible(grid_t* grid, gridcell_t* player, gridcell_t* target);
void grid_generateGold(grid_t* grid, int minPiles, int maxPiles, int goldTotal);
void grid_delete(grid_t* grid );
```


### Detailed pseudo code


#### `grid_new`
```
Check for null arguments
Allocate memory for new grid struct
Return the grid
```


#### `grid_get_NR`
```
Check for null arguments
Return the number of rows
```


#### `grid_get_NC`
```
Check for null arguments
Return the number of columns
```


#### `grid_get_gridarray`
```
Check for null arguments
Return grid->gridarray
```


#### `grid_get_map`
```
Check for null arguments
Return grid->map
```


#### `grid_load`
```
Check for null arguments
open file
get number of lines (rows)
get number of columns (length of first line)
for each line in file
   for each character in line
       Concatenate it to the map string
       create new gridcell
       if it is a wall, set wall=true
       if it is a room, set room=true
   Put a newline character onto the map string
```


#### `grid_set`
```
Check for null arguments
Calculate the index in gridarray that corresponds to the x,y position
Gridcell_set that gridcell to the corresponding character
Change the character at the map’s corresponding index
```


#### `grid_isVisible`
```
Check if grid, player, or target is null
If player and target are the same point, return true
If they are on a vertical line (dx == 0)
   Loop over y values in between
       If there is a wall
           Return false
If they are on a horizontal line (dy == 0)
   Loop over x values in between
       If there is a wall
           Return false


Set the incrementation value for x and y (if x is moving up/down, increment negative/positive)
Loop over all the integer x values from player_getX to target_getX
   At each integer x value, calculate the corresponding y value
   Take the floor and the ceiling of each y value
   Get the gridcells at (x, underY) and (x, overY)
   If either is a wall
       Return false
Loop over all the integer y values from player_getY to target_getY
   At each integer y value, calculate the corresponding x value
   Take the floor and the ceiling of each x value
   Get the gridcells at (underX, y) and (overX, y)
   If either is a wall
       Return false


Return true
```


#### `grid_get`
```
check for null arguments
calculate index from x,y position
return gridcell at that index
```


#### `grid_delete`
```
check for null argument
call gridcell_delete to free each gridcell in gridarray
free the map
free the gridarray
free the grid
```
  
### Data Structures
#### gridcell_t
The `grid_t` struct also uses `gridcell_t`, a struct that represents one specific cell in the grid. The gridcell struct holds certain members in it, which include:


```c
typedef struct gridcell {
 char c;                       // actual makeup of cell
 int x;                        // x position in grid
 int y;                        // y position in grid
 int gold;                     // amount of gold in place, 0 if none
 bool show;                    // if cell can be seen by player or not
 bool room;                    // is the gridcell is a room?
 bool isWall;                  // is the gridcell a wall?
} gridcell_t;
```


The struct also implements getter and setter functions for each of these members.


## Server


### Data structures


`gameData` is a local structure used to store game data, including the master grid, array of players, number of players, the address of the spectator, a boolean of whether or not there is a spectator, the number of remaining gold, the number of rows, the number of columns, and the address of the player that just found gold. An instance of gameData is created as a global variable.


```c
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
```


Within `gameData`, the structures *grid_t*, *player_t*, and *addr_t* are also used. The grid is used to store the game map, the player is used to create the player array, and address is used to store the addresses of players/spectator. More details about grid and player can be found above. *addr_t* was provided in the message module in the support library.


### Definition of function prototypes


`main` accepts command-line arguments, initializes global gameData variable, initializes the message module, and calls the message_loop function.


`parseArgs` parses and validates the command-line arguments, extracting the map file name and seed if given.


`handleMessage` handles incoming messages from clients and calls appropriate methods based on game logic.


`addPlayer` creates a new player and adds them to the player array in the global gameData struct.


`addSpectator` creates a spectator and stores their client address in the global gameData struct.


`handleKey` uses switch cases to pass appropriate parameters to moveOnMap based on which key player presses and where they want to move.


`moveOnMap` changes the game map based on parameters passed to it by handleKey.


`handleQuit` deactivates a player or spectator if they press the "Q" key.


`dropGold` drops a random number of gold piles (between minimum and maximum number of gold piles) in the map.


`updatePlayers` sends GOLD and DISPLAY messages to the clients of all the players.


`updateSpectator` sends GOLD and DISPLAY messages to the spectator if there is one.


`gameOver` creates the game summary and sends it as a message to all clients.


Function prototypes:
```c
int main (const int argc, char* argv[]);
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
```


### Detailed pseudo code


> For each function write pseudocode indented by a tab, which in Markdown will cause it to be rendered in literal form (like a code block).
> Much easier than writing as a bulleted list!
> For example:


#### `main`:
```
call parseArgs
get number of rows and columns in map by looping through the map file
initialize gameData variable using map file and default values
intialize the message module
call message_loop and pass handleMessage as a parameter to accept and process messages from clients
shut down message module
clear memory for grid and players
```


#### `parseArgs`:
```
check if there are 2 or 3 arguments, otherwise log error
check if map file opens properly
if given, check if seed is a valid integer
```


#### `handleMessage`:
```
if message begins with "PLAY "
   parse player name
   call addPlayer
else if message begins with "SPECTATE "
   call addSpectator
else if message begins with "KEY "
   parse keystroke
   if client address if not spectator
       use address to identify player that sent message by looping through player array
       call handleKey, passing the moving player and keystroke
       update player's visibility
   else
       check if keystroke is "Q" for quit
       handle quit for spectator
```


#### `addPlayer`:
```
if number of players exceeds max
   send QUIT message
else if name is empty
   send QUIT message
else
   ensure name length is within bounds and replace invalid characters
   get character for player
   create new player
   if player is not null
       add player to player array
       increment number of plaers
       send OK message to client
       send GRID message to client
       drop player in randomly generated room location in map
   update player visibility
```


#### `addSpectator`:
```
if there is already a spectator
   send QUIT message to existing spectator
set game's spectator address to given address
set game's hasSpect boolean to true
send GRID message to new spectator client
```


#### `handleKey`:
```
switch cases for every possible keystroke
   if h, l, j, k, y, u, b,  or n
       call moveOnMap with appropriate position change
   if capital letters
       while moveOnMap returns true
           update player visibility
   if Q
       call handleQuit
   else
       log error for unknown keystroke
       send error message to client
```


#### `moveOnMap`:
```
if player is active
   get player coordinates
   get gridcell player is in (curCell) and gridcell of cell they are trying to move to (newCell)
   get the player's character
   if both cells are not null
       get char of newCell(newChar)
       if newChar is an uppercase letter
           swap the letters in newCell and curCell
           return true
       else if newChar is '*'
           get amount of gold in pile
           add gold amount to player's score
           subtract gold in pile from game's remaining gold
           move player to new position
           change char in old position to '.' or '*'
           send GOLD message to player
           return true
       else if newChar is '.' or '*'
           move player to new position
           change char in old position to '.' or '*'
           return true
return false
```


#### `handleQuit`:
```
if player's address matches spectator's address
   change hasSpect boolean to false
   send QUIT message to spectator
else
   remove player's symbol from map
   deactivate player
   send QUIT message to player
```


#### `dropGold`:
```
generate a random number of piles between min and max number of piles
keep variable to track remaining gold to drop
keep variable for upper bound of amount of gold in each pile
for loop to drop generated number of piles
   generate random number of gold in one pile
   boolean dropped is false
   while not dropped
       generate random map coordinates
       if map location is a room spot
       set the location in map to a gold pile with the generated number of gold
   subtract number of gold in the pile from the remaining amount to be dropped
drop any left over gold in random room spot
```


#### `updatePlayers`:
```
for loop through all players
   if player is not the one that just found gold
       send GOLD message
   send DISPLAY message
```


#### `updateSpectator`:
```
if game has a spectator
   send GOLD message
   send DISPLAY message
```


#### `gameOver`:
```
create game over message
send game over message to all players
send game over message to spectator
```
---





## Client


### Data structures


> For each new data structure, describe it briefly and provide a code block listing the `struct` definition(s).
> No need to provide `struct` for existing CS50 data structures like `hashtable`.


The client uses one data structure that is used as a static global variable, the `data` struct, that is used for convenience to store game critical information only received once from the server. The `data` construct contains three variables, `int NROWS` and `int NCOLS` that store the required window size passed from the server and `char player` which is a flag to indicate spectator functionality if zero and an alphabetical character if storing player’s tag as seen by the server otherwise.


The client also uses the `addr_t` struct declared by `message.h` in the support module.


### Definition of function prototypes


> For function, provide a brief description and then a code block with its function prototype.
> For example:


stdin has input ready (cbreak mode), reads a char and sends it to the server.
Returns true if the message loop should exit, such as during an error, and otherwise returns false.
```c
static bool handleInput(void* arg);
```


Receive datagrams from server at addr_t `from`, `message` is parsed with the use of helper functions
```c
static bool handleMessage(void* arg, const addr_t from, const char* message);
```


Fills data’s player field with value depending on player or spectator response type from server
```c
handleOK(const char* message);
```                      */
Parses message from server expecting GRID %d %d syntax, verifies that CURSES window is at least the required size, prompting the user to resize if not.
```c
static bool handleGRID(const char* message);
```


Parses message of type GOLD %d %d %d, displays gold values in the topline of CURSES window. If the display line is longer than the window, it is cropped.
```c
static bool handleGOLD(const char* message);
```


Parses message from server of the type DISPLAY\nstring, displays the chars following '\n' character in a CURSES window.
```c
static void handleDISPLAY(const char* message);
```


Initializes curses, creating window, gathering window size, and setting terminal to appropriate settings
```c
static void initialize_curses(); // CURSES
```


Fills ncurses screen with bank character ' '
```c
static void init_map();
```


Display the map (char* display) into CURSES screen
```c
static void display_map(char* display);
```


display temp status message to the right of the gold status message
```c
static void display_temp_message(const char* temp);
```


clears temp status message to the right of the gold status message
```c
static void clear_temp_message();
```


allocates memory for data struct, initializes to default values
```c
static data_t* data_new();
```


### Detailed pseudo code


> For each function write pseudocode indented by a tab, which in Markdown will cause it to be rendered in literal form (like a code block).
> Much easier than writing as a bulleted list!
> For example:


#### `handleMessage`:


   validate arguments
   Check message type:
   if of type “OK”:
	evokes handleOK
	if hadnleOK fails, clear memory and quit
   if of type “GRID”:
	evoke handleGRID
	if handleGRID fails, clear memory and quit 
   if of type “GOLD”:
	evoke handleGOLD
   if of type “DISPLAY”:
	evoke handleDISPLAY
   if of type “QUIT”:
	exit curses
	print quit message to stdout
	return true
   if of type “ERROR”:
	print error message to stderr
	Display error in CURSES window
   else
       print the malformed message to stderr
   return false


#### `handleInput`:
   validate arguments
   	in case of errors, print to stderr and terminate loop
   get char from stdin
   form a message string of form “KEY “ + char
   clear temporary statuses with clear_temp_message
   return false


#### `initialize_curses`:
   Initialize screen
   Get number of rows and columns in screen
	Pass this to NROWS and NCOLS data fields in data
   Set stdin to cbreak mode
   Set typed characters to not show
   Set cursor to be invisible
   Set desired display settings and color attributes


#### `init_map`:
   Gets max number of rows and columns from CURSES screen
   Fill entire CURSES screen with blank character ‘ ‘
   Call refresh


#### `display_map`:
   Iterate through every row in screen while row is less than value stored in data
	For every row, iterate through every column while less than value in data
		Move to the coordinate defined by row and column, skipping the very first (occupied by status message)
		if flattened 1d coordinate from row and column is within range of char* display, add the char at the flattened coordinate to teh screen at the 2d location
		else, fill the cooridante with an empty character ‘ ‘
   Evoke refresh


#### `display_temp_message`:
   Gets max number of columns from CURSES screen
   Iterate through top row of CURSES screen until char on screen matches ‘.’, marking end of the gold status message
   Break loop to save this index
   ensure new temporary message is not null
	Print temporary message to the right of the found index on CURSES screen
   evoke refresh


#### `clear_temp_message`:
   Gets max number of columns from CURSES screen
   Iterate through top row of CURSES screen until char on screen matches ‘.’, marking end of the gold status message
   Break loop to save this index
   replace every character on screen with ‘ ‘ to the right of the found index
   evoke refresh


#### `handleOK`:
   Set player field of data to 0
   if message is not null
if sscanf is of format “OK %c”
Set player field of data to %c instead
else return false
   evoke initialize_curses
   evoke init_map
   return true
    
#### `handleGRID`:
   if message not fo form “GRID %d %d”
	Log error to stderr
return false
   else read the trailing ints into nrows and ncols respectively
   while CURSES screen is not at least nrows+1 in height and at least ncols in width
	close curses window
	Open new clear CURSES window
       print error message to stderr
	inform user on screen to resize window
	Await input char from user
	if input is of type ‘\n’
		Close window
		Create new window with initialize_curses
	  else continue loop
   Set NROWS data field of data to nros+1
   Set NCOLS data field of data to ncols
   Return true


#### `handleGOLD`:
   if message in format “GOLD %d %d %d”
	Read trailing ints to gold_collected, gold_purse, gold_left
	Get width of CURSES screen
	Starting from 0 to the width of the screen, loop and replace every
	Index at row 0 on screen with a blank char ‘ ‘
	if player field in data is 0
		Fill string buffer with spectator status message
	else 
		fill buffer with gold status message
		if gold_collected is non-zero
			Append to buffer temporary gold collected message
	print string buffer to CURSES screen at position 0, 0, reading up to width of screen characters from buffer
	evoke refresh
	return true
   else 
	Log malformed string error to stderr
	Return false
 


#### `handleDISPLAY`:
   Using length of “DISPLAY\n”, read characters following the length of that string into a buffer
   Pass the buffer string into display_map function call


#### `data_new`:
   Allocate memory for data_t struct type
   if memory allocation failed (data is NULL)
	exit(3)
   else 
	Set NROWS field of data to -1
	Set NCOLS field of data to -1
	Set player field of data to 0
   return data


---


## Testing plan


### unit testing
We tested the modules in the ‘player’ library. We implemented a ‘gridtest.c’ file, whose output is documented in ‘gridtest.out’, which tests the loading and printing of various grids. We also implemented a ‘visibilitytest.c’, whose output is documented in ‘visibilitytest.out’, which tests the functionality of visibility in the ‘grid’ module. It loads the ‘visdemo’ grid and creates various gridcells, testing visibility between them. To run both testing files with Val grind, run ‘make test’ in the player directory.


### integration testing
We will test the complete main programs by first connecting our server with the given client code, then debugging with the correct client so we know the issues lie within the server. Once we have the server up to specs, we will use the given server to debug our client code, starting with sending simple messages between our client and their server.


Our client was tested by connecting it to the example server provided to us. We also tested it by hand by playing the game with valgrind evoked from the command line. No memory leaks were found except for those created by NCURSES. 

### system testing
Once both our server and client are up to specs, we will run the server and connect our own version of the client to it. We will play the game and locate and fix any errors.
