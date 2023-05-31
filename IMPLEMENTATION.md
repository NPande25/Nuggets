# CS50 Nuggets
## Implementation Spec
### CecsC (Team 17), Spring, 2023

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our implementation also includes `player`, `grid`, and `gridcell` modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

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

A function to create a new player

```c
player_new()
```

A function to update a player's visibility grid (boolGrid) based on the main grid after each player moves. 

```c
void player_playerVisibility(player_t* player, grid_t* grid);
```

A function to take the current grid and print out the string map that the specific player sees theirselves.

```c
char* player_get_string(player_t* player, grid_t* grid);
```

A function to delete a player

```c
player_delete()
```

### Detailed pseudo code

#### `player_playerVisibility`:

	for each gridcell in the main map grid
		checks if each cell is visable from the player's current position
		if yes, mark that index as true
		if not, mark as false

---

#### `player_get_string`:

	for each gridcell in the main map grid
		if the gridcell is visible to player
			if it is gold
				if the player can currently seen it
					show the gold
				otherwise show it as an empty room space
			else if it is the player's character
				show it as @ symbol
			otherwise, print the gridcell as it comes

## Grid

### Data structures

Grid does not use any outside data structures. It uses `gridcell`, which is a sub-data structure described below.

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
grid_new()
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
	

## Server

### Data structures

> For each new data structure, describe it briefly and provide a code block listing the `struct` definition(s).
> No need to provide `struct` for existing CS50 data structures like `hashtable`.

We use three data structures: an array of `player`s, which contains data for each player, a `grid` that stores the game map, and `gridcell` which stores data for an individual cell in the grid.

### Definition of function prototypes

> For function, provide a brief description and then a code block with its function prototype.
> For example:

A function to parse the command-line arguments
```c
static int parseArgs(const int argc, char* argv[]);
```

A function to initialize the game struct
```c
static void initializeGame(char* mapFileName, const int seed)
```

A function to update all players when changes on the map occur
```c
static void updatePlayers(players_t* allPlayers, grid_t* map)
```

A function to update all clients when changes on the map occur
```c
static void updateClients(clients_t* allClients)
```

A function to display the game over message and player stats
```c
static void gameOver(players_t* allPlayers)
```

### Detailed pseudo code

> For each function write pseudocode indented by a tab, which in Markdown will cause it to be rendered in literal form (like a code block).
> Much easier than writing as a bulleted list!
> For example:

#### `parseArgs`:

	validate commandline
	verify map file can be opened for reading
	if seed provided
		verify it is a valid seed number
		seed the random-number generator with that seed
	else
		seed the random-number generator with getpid()

---

## XYZ module

> For each module, repeat the same framework above.

### Data structures

### Definition of function prototypes

### Detailed pseudo code

---

## Testing plan

### unit testing

> How will you test each unit (module) before integrating them with a main program (client or server)?

Grid module: for the grid module, we will write a grid_test program to test the grid functions such as 

### integration testing

> How will you test the complete main programs: the server, and for teams of 4, the client?

We will test the complete main programs by first connecting our server with the given client code, then debugging with the correct client so we know the issues lie within the server. We will start by trying to send simple messages between server and client, and then progressing to having the client join as a player on the server's game.

Once we have the server up to specs, we will use the given server to debug our client code, starting with sending simple messages between our client and their server, and then progressing to having the client join as a player on the server's game. 

### system testing


