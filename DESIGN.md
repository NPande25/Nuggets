# Design Spec
## June 2023

In this document, we discuss our design decisions for the CS50 Nuggets game, which are independent of the implementation. The Design spec may include many features, but here we focus on the main subset:
* Assumptions
* User interface (including command-line parameters and stdin/stdout/graphical interface);
* Inputs and outputs
* Functional decomposition into modules
* Pseudo code (plain English-like language) for logic/algorithmic flow
* Major data structures
* Testing plan, including unit tests, integration tests, system tests.

Our design includes modules for `grid`, `gridcell`, and `player`. We describe each program and module separately. We do not describe the `support` library nor the modules that enable features that go beyond the spec. We avoid repeating information that is provided in the requirements spec.

## Client

The *client* acts in one of two modes:

 1. *spectator*, the passive spectator mode described in the requirements spec.
 2. *player*, the interactive game-playing mode described in the requirements spec.

### User interface

The user will see a CURSES display, the top line of which shall provide game status, and the rest of the rows will display the game state from the client’s perspective. 

The user interfaces with the game by sending keystrokes input through stdin.

See the requirements spec for both the command-line and interactive UI.

### Inputs and outputs

The client is launched from the command line, taking in 2 or 3 arguments:
```
./client hostname port [playername]
```
`hostname` IP address where the server is running

`port` Port number on which the server expects messages

`playername` (optional) If supplied, used as the players name. The client flags a missing playername, instead joining as a spectator

#### Inputs

Player keystrokes: All keystrokes are sent immediately to the server. The valid keystrokes for the player are

Q quit the game.

h move left, if possible

l move right, if possible

j move down, if possible

k move up , if possible

y move diagonally up and left, if possible

u move diagonally up and right, if possible

b move diagonally down and left, if possible

n move diagonally down and right, if possible

H move as far left as possible

L move as far right as possible

J move as far down as possible

K move as far up as possible

Y move as far diagonally left and up as possible

U move as far diagonally up and right as possible

B move as far diagonally down and left as possible

N move as far diagonally down and right as possible

#### Outputs

Terminal Display: Outputs the sections of the map that is visible to the player to the terminal along with client data (gold found by player) and global data (total gold not yet found).

### Logging

When evoked with the syntax ./client 2>player.log, important messages for understanding the bahviour of the client are logged to player.log. Logging to an external file is the recommended way to launch the client, to avoid error messages appearing on screen.

Instead, critical information for gameplay, including certain errors, are logged as temporary messages to the status line on the top of the screen.

When the game terminates, or an error arises, the log file is saved.

### Functional decomposition into modules

Modules:

* message module (included): facilitates sending client input from stdin to the server
* ncurses module (included): facilitates writing and reading from a screen

There are no othe modules other than the main module, facilitating functionality for player operations and differentiating between player & spectator functionality
 
### Pseudo code for logic/algorithmic flow

The client will run as follows:

	execute from a command line per the requirement spec
	parse the command line, validate parameters
	Initialize global data variable
	If playername is present
		Send player message to server
	else
		Send spectator message to server
	evoke message look with handleInput, and handleMessage
	Free memory of data
	endwin, close curses
	message_done, close message

#### handleInput()

	reads a single character input from stdin without delay
	clears UI screen’s top line
	sends char as a key message to the server

#### handleMessage()

	verifies parameters
	sorts incoming message form server by type:
	If the type is valid (OK, GRID, GOLD, DISPLAY, ERROR, QUIT)
		evoke corresponding handle helper function to update UI
	else
		Ignore the malformed message and log it for debugging

	In case of a terminal error, break the message loop


### Major data structures

There is one data struct included, a global static variable called data. This struct contains three primitive data types which store important information that needs to be remembered between message loops such as screen info and the player’s assigned symbol.

---

## Server
### User interface

As described in the Requirements Spec, the server’s only interface with the user is on the command-line; it must have one or two arguments.
 
```
$ ./server map.txt [seed]
```

The first argument is the pathname for a map file and the second argument is an optional seed for the random-number generator; if provided, the seed must be a positive integer.

### Inputs and outputs
*Input*: There are no inputs, only command-line parameters described above.
*Output*: The server outputs a game summary including player names and scores when the game is over. The server also logs useful information to stderr.

### Functional decomposition into modules

We anticipate the following modules or functions:
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


### Pseudo code for logic/algorithmic flow

The server will run as follows:

	execute from a command line per the requirement spec
	parse the command line, validate parameters
	to set up data structures
	initialize the 'message' module
	print the port number on which we wait
	call message_loop(), to await clients
	call gameOver() to inform all clients the game has ended
	clean up

#### handMessage()
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

### Major data structures
`grid_t`: used to store the master grid.
`gridcell_t`: used to store data for each grid cell in the grid.
`player_t`: used to create player array and store data for game players.

## grid_t

The grid module represents a specific instance of the map, which is to be initialized as a variable under each player_t and updated after each relevant keystroke input from players. The `grid_t` struct implements the following functions:

### Functional decomposition

`grid_new`, create a grid struct

`grid_delete`, delete a grid struct

`grid_init`, load a map from a file into a grid

`grid_set`, set a certain cell in the grid to a specific character

`grid_iterate`, goes through each grid cell and performs a specific function

`grid_generateGold`, generates all the gold throughout the map

`grid_updateGold`, updates the amount of gold at a specific location

`grid_getGold`, returns the amount of gold at a specific location

`grid_isVisible` determines whether a gridcell is visible from a certain location


### Pseudocode for logic/algorithmic flow
Pseudocode for non-trivial, major functions.

**grid_init**
```
Read the map file by character
Create new gridcell for each character
Store character in gridcell->char
```

**grid_set**
```
calculate which index in array will correspond to x,y location
set that specific gridcell to the character
```

**grid_insertGold**
```
use random number generator to generate number of piles
for each pile
   randomly choose location in the map that isn't blank or wall
   calculate the index in the gridArray that the location coordinates correspond to
   use random number generator to calculate amount of nuggets in pile
   subtract this number from the remaining amount of gold to place
   use gridcell_setGold to put the gold in
```

**grid_updateGold**
```
calculate the index in the gridArray that the location coordinates correspond to
use gridcell_setGold to put the gold into the gridcell
```

**grid_isVisible**
```
Calculate the difference between the player's position and the target cell's position
Divide the distance into smaller steps
Calculate the step sizes
For each step from 0 to numSteps - 1:
   Update the current position by incrementing it with the step sizes.
   Round the current position to the nearest grid cell coordinates (x, y).
   Check if the cell is outside the map boundaries
   Check if the cell at (x, y) in the map is a wall. If so--return false
   If it's not the first step (i > 0), calculate the previous position (prevX, prevY) by subtracting the step sizes from the current position.
   Check if both the previous cell (prevX, prevY) and the current cell (x, y) are walls in the map. If they are, it means the path is blocked on both sides, so consider the cell not visible and return false.
If no intersections with walls were found during the loop, consider the cell visible and return true.
```

### Major data structures
No major data structures from libcs50 are used in `grid_t`. However, we implement a `gridcell_t` helper module that populates each cell in the grid. This is outlined below.


## gridcell_t

This module is a helper module for `grid_t`, representing a specific cell of the grid, which is to be initialized as each position in a new grid. Each gridcell will be updated after each relevant keystroke input from players. The functions are breifly described below, and

### Functional Decomposition
`gridcell_new`, which creates a new gridcell
`gridcell_delete`, which deletes a gridcell struct
`gridcell_set`, which sets a gridcell to a certain character (wall, blank, gold, etc)
`gridcell_getX` gets x position of a grid cell
`gridcell_getY` gets y position of a grid cell
`gridcell_setGold` sets the amount of gold for a gricell

### Pseudocode for logic/algorithmic flow
Most functions are quite trivial, simply updating the relevant properties, which are below:

### Major Data Structures
There are no major data structures used in gridcell.

```c
char c;
int x;
int y;
bool show;
int nuggets;
```

### player_t*

module/struct that represents one player, which is to be initialized once a new client makes a connection and joins the game. Each player struct has the following functions:

`player_new(char c, char* name)`

`void player_loadmap(player_t* player, char* map)`

`void player_deactivate(player_t* player)`

`void player_delete(player_t* player)`

`void player_move(player_t* player, player_t* mover, char keystroke)` 


#### void player_move
```
if valid keystroke
      if player != mover
           modify where mover character is on player's grid based on keystroke
      if player == mover
	modify where '@' symbol is on player's grid based on keystroke
	update visibility based on movement
```


### Major data structures

No major data structures for the player module.


