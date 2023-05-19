# CS50 Nuggets
## Design Spec
### Team name, term, year

> This **template** includes some gray text meant to explain how to use the template; delete all of them in your document!

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes x, y, z modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

## Player

> Teams of 3 students should delete this section.

The *client* acts in one of two modes:

 1. *spectator*, the passive spectator mode described in the requirements spec.
 2. *player*, the interactive game-playing mode described in the requirements spec.

### User interface

See the requirements spec for both the command-line and interactive UI.

> You may not need much more.

### Inputs and outputs

> Briefly describe the inputs (keystrokes) and outputs (display).
> If you write to log files, or log to stderr, describe that here.
> Command-line arguments are not 'input'.

### Functional decomposition into modules

> List and briefly describe any modules that comprise your client, other than the main module.
 
### Pseudo code for logic/algorithmic flow

> For each function write pseudocode indented by a tab, which in Markdown will cause it to be rendered in literal form (like a code block).
> Much easier than writing as a bulleted list!
> See the Server section for an example.

> Then briefly describe each of the major functions, perhaps with level-4 #### headers.

### Major data structures

> A language-independent description of the major data structure(s) in this program.
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
*initializeGame*, which sets up the data structures, including dropping gold randomly in the map
*handleMessage*, helper method for message_loop() that processes messages from clients
*updatePlayers*, which updates all players whenever any player moves or gold is collected
*updateClients*, which sends out updated map to all active clients after any player move
*gameOver*, which sends out a summary that includes player names and scores when game is over

### Pseudo code for logic/algorithmic flow

The server will run as follows:

	execute from a command line per the requirement spec
	parse the command line, validate parameters
	call initializeGame() to set up data structures
	initialize the 'message' module
	print the port number on which we wait
	call message_loop(), to await clients
	call gameOver() to inform all clients the game has ended
	clean up

#### initializeGame()
initializeGame sets up grid and drops gold piles into random cells. See IMPLEMENTATION.md for the pseudocode.

#### handMessage()
handleMessage determines what the program will do based on the user keystrokes. See IMPLEMENTATION.md for pseudocode.

### Major data structures
Helper modules provide all the data structures we need:
* *player* contains all the data needed for each player
* *grid* contains the map
* *gridcell* contains data for each cell in the grid

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
