/*
deals with the grid module, which is in charge of modifying and returning strings to print the correct map to the terminal

The right representation of a grid can make your code vastly simpler than it would be if you choose other representations. Consider:

What information must the spectator keep about the state of the game?
- all the players and their positions
-all the gold and its positions
- the entire map string, nothing hidden

What information must the player keep about the state of the game?
- all the players and their positions
-all the gold and its positions
- what's hidden and what's not

What information must the server keep about the state of the game?
- - all the players and their positions
-all the gold and its positions
- what's hidden and what's not

What information does the string below record about the state of the game?
- a string will only put out the literal characters in the map
-need a separate array of gridCell items to keep track of what's hidden, etc.

What information does the string below not record about the state of the game?
-need a separate array of gridCell items to keep track of what's hidden, etc.

How much gold is in each pile... and does any player know until they step on a pile?
-random sized piles, so they won't know until they step on it

Do these questions give you ideas about how to store game state?
-have to create gridCell object
-should have
    char c - literal character
    int gold - amount of gold in pile if it is gold pile
    bool show - character hidden from view or not?

char* grid = ...;   // a string variable
puts(grid);         // produces the output below
    puts(grid) will loop through gridCell array, if show, then put c, if !show, put blank space
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "gridcell.h"

/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct grid {
  gridcell_t** gridarray;       // array of gridcells in the grid
  int NR;                       // number of rows
  int NC;                       // number of columns
} grid_t;


//display consists of NR+1, NC (to fit text header)                 //note: do i need to allocate here or within gridcell_new?
grid_t* grid_new(int NR, int NC) {
  // Allocate memory for the grid structure
  grid_t* grid = (grid_t*)malloc(sizeof(grid_t));
  
  // Allocate memory for the rows of the gridarray
  grid->gridarray = (gridcell_t**)malloc(NR * NC * sizeof(gridcell_t*));

  // Return the initialized grid
  return grid;
}

//loads given map into grid
void grid_load(grid_t* grid, char* map) {
  
}