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


## grid_t

module/struct that represents a specific instance of the map, which is to be initialized as a variable under each player_t and updated after each relevant keystroke input from players. The `grid_t` struct implements the following functions:

### Functional decomposition

`grid_new`, create a grid struct
`grid_delete`, delete a grid struct
`grid_init`, load a map from a file into a grid
`grid_set`, set a certain cell in the grid to a specific character
`grid_iterate`, goes through each grid cell and performs a specific function
`grid_generateGold`, generates all the gold throughout the map
`grid_updateGold`, updates the amount of gold at a specific location
`grid_getGold`, returns the amount of gold at a specific location

### Pseudocode for logic/algorithmic flow
Pseudocode for non-trivial, major functions

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

### Major data structures
No major data structures from libcs50 are used in `grid_t`. However, we implement a `gridcell_t` helper module that populates each cell in the grid. This is outlined below.



## gridcell_t

This module is a helper module for `grid_t`, representing a specific cell of the grid, which is to be initialized as each position in a new grid. Each gridcell will be updated after each relevant keystroke input from players, and has the following functions:

### Functional Decomposition
```c
gridcell_new(char c);
gridcell_delete( gridcell_t*);
gridcell_set(char c);
gridcell_getX();
gridcell_getY();
gridcell_setGold()
gridcell_isVisible;
```

variables:
```c
char c
bool show
int nuggets
```

