# CS50
## Design Spec
### CecsC, Spring, 2023

> This **template** includes some gray text meant to explain how to use the template; delete all of them in your document!

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes x, y, z modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

## Player

The *client* acts in one of two modes:

 1. *spectator*, the passive spectator mode described in the requirements spec.
 2. *player*, the interactive game-playing mode described in the requirements spec.

### User interface

The user will see a display, the top line of which shall provide game status, and the rest of the rows will display the game state from the client’s perspective. 

The user interfaces with the game by sending keystrokes input through stdin.

See the requirements spec for both the command-line and interactive UI.

> You may not need much more.

### Inputs and outputs

> Briefly describe the inputs (keystrokes) and outputs (display).
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

Terminal Display: Outputs the sections of the map that is visible to the player to the terminal along with client data (gold found by player) and global data (total gold not yet found).

> If you write to log files, or log to stderr, describe that here.

When an event occurs, it will be written to a log file. Events include: a client joining the server, a client finding a gold nugget, and a client leaving the server. When the game terminates, the log file is saved. In the case of an error, the last coordinates of every client as well as the last input from the client.

> Command-line arguments are not 'input'.

### Functional decomposition into modules

> List and briefly describe any modules that comprise your client, other than the main module.

Modules:
* message module (included): facilitate sending client input from stdin to the server
* player module: facilitate functionality for player operations and differentiating between player & spectator functionality
 
### Pseudo code for logic/algorithmic flow

> For each function write pseudocode indented by a tab, which in Markdown will cause it to be rendered in literal form (like a code block).
> Much easier than writing as a bulleted list!
> See the Server section for an example.

The client will run as follows:

	execute from a command line per the requirement spec
	parse the command line, validate parameters
	call initialize_client() to initialize required modules
	initialize the 'message' module
	verifies screen as per requirements 
call connect_client()
		call message_loop() to handle incoming messages from server
connect to server at port received	
	call game_loop(), await key strokes from player
		call move_clean() to clean player input
		call message_send() to send clean keystrokes to server
		call display() to print grid and client info to terminal
	terminate modules and clean up

> Then briefly describe each of the major functions, perhaps with level-4 #### headers.

#### initialize_client()

* initializes the `message` module
* initializes the `player` module
* verify screen is sufficient for display as per requirments

#### conncet_client()

* message_loop() handles incoming messages from the server, initially awaiting server port to connect
* sends message to server to verify connection

#### game_loop()

* reads from stdin one character at a time until termination from server or client
* calls move_clean() to filter input for expected keystrokes
* calls display() to print grid from server into terminal

### Major data structures

> A language-independent description of the major data structure(s) in this program.

#### player_t*

module/struct that represents one player, which is to be initialized once a new client makes a connection and joins the game. Each player struct has the following functions:

player_t* player_new(char c, char* name)
void player_loadmap(player_t* player, char* map)
void player_deactivate(player_t* player)
void player_delete(player_t* player)
void player_move(player_t* player, char direction)

variables:


#### grid_t*

module/struct that represents a specific instance of the map, which is to be initialized as a variable under each player_t and updated after each relevant keystroke input from players. Each grid struct has the following functions:

grid_new(int numRows, int numCols )
grid_delete( grid_t* grid )
grid_init( grid_t* grid, char* map )
grid_set( int x, int y, char c )
char* grid_iterate()
go thru each grid cell, either print c or blank space based on bool show

grid_insertGold(grid_t* grid, counters_t* goldPiles)

variables:
gridcell_t** gridArray;



#### gridcell_t*

module/struct that represents a specific cell of the grid, which is to be initialized as each position in a new grid. Each gridcell will be updated after each relevant keystroke input from players, and has the following functions:

`gridcell_new( )`
`gridcell_delete( gridcell_t*)`
`gridcell_set(char c)`
`gridcell_getX()`
`gridcell_getY()`
`gridcell_isVisible(player_t* player, gridcell_t* gridcell)`

variables:
char c
bool show


> Mention, but do not describe, any libcs50 data structures you plan to use.

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
*main*, which parses arguments and initializes other modules
*parseArgs*, which parses the command-line arguments
*initializeGame*, which sets up the data structures, including dropping gold randomly in the map, initializes message module and announces port number and accepts messages from clients
*processMsg*, which processes messages from clients
*updatePlayers*, which updates all players whenever any player moves or gold is collected
*updateClients*, which sends out updated map to all active clients after any player move
*sendSummary*, which sends out a summary that includes player names and scores when game is over

### Pseudo code for logic/algorithmic flow

parseArgs
```
initialize file using map file name
if file exists
close file
else
log error and exit nonzero

if seed is given
save seed value
```

initializeGame
```
create array of players
create and initialize map using mapfile
drop gold into random positions in map

while there is still gold left in the map
	wait for messages sent from clients
	call processMsg

call sendSummary to print game over summary
```
processMsg
```
if player moves
	store players new position
check if score changed (player found gold)
update main map
if player leaves
deactivate player 
call updatePlayers
call updateClients
```

updatePlayers
```
for all players in game
update map, scores, and positions
```

updateClients
```
for all player in game
send map or message to client and display
```

sendSummary
```
print game over message
for all players in game
print player details
```
> For each function write pseudocode indented by a tab, which in Markdown will cause it to be rendered in literal form (like a code block).
> Much easier than writing as a bulleted list!
> For example:

The server will run as follows:

	execute from a command line per the requirement spec
	parse the command line, validate parameters
	call initializeGame() to set up data structures
	initialize the 'message' module
	print the port number on which we wait
	call message_loop(), to await clients
	call gameOver() to inform all clients the game has ended
	clean up


> Then briefly describe each of the major functions, perhaps with level-4 #### headers.

### Major data structures
Grid
GridCell
Player
Client

> Describe each major data structure in this program: what information does it represent, how does it represent the data, and what are its members.
> This description should be independent of the programming language.
> Mention, but do not describe, data structures implemented by other modules (such as the new modules you detail below, or any libcs50 data structures you plan to use).

---

## XYZ module

> Repeat this section for each module that is included in either the client or server.

### Functional decomposition

> List each of the main functions implemented by this module, with a phrase or sentence description of each.

### Pseudo code for logic/algorithmic flow

> For any non-trivial function, add a level-4 #### header and provide tab-indented pseudocode.
> This pseudocode should be independent of the programming language.

### Major data structures

> Describe each major data structure in this module: what information does it represent, how does it represent the data, and what are its members.
> This description should be independent of the programming language.


