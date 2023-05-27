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
#include <math.h>
#include "grid.h"
#include "file.h"
#include "mem.h"
#include "gridcell.h"

/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct grid {
  gridcell_t** gridarray;       // array of gridcells in the grid
  char* map;
  int NR;                       // number of rows
  int NC;                       // number of columns
} grid_t;


//display consists of NR+1, NC (to fit text header)                 //note: do i need to allocate here or within gridcell_new?
grid_t* grid_new(int NR, int NC) {
  // Allocate memory for the grid structure
  grid_t* grid = mem_assert(malloc(sizeof(grid_t)), "grid memory error");
  
  // Allocate memory for the rows of the gridarray
  grid->gridarray = mem_assert(malloc(NR * NC * sizeof(gridcell_t*)), "gridarray memory error");

  // Return the initialized grid
  return grid;
}



/* loads given file into grid. See 'grid.h' for more info. */
void grid_load(grid_t* grid, char* pathName) 
{
  // check arguments
  if (grid == NULL || pathName == NULL) {
      fprintf(stderr, "Null grid or Pathname");
      return;
  }

  // open file, check if worked
  FILE* fp;
  fp = fopen(pathName, "r");
  if (fp == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", pathName);
    return;
  }
   
  // get number of columns
  char* lineOne = file_readLine(fp);
  int numCols = strlen(lineOne);
  grid->NC = numCols;
  rewind(fp);
  free(lineOne);

  //get number of rows
  int numRows = file_numLines(fp);
  grid->NR = numRows;

  // for each character, concatenate to end of string with strncat
  char* map = malloc(numRows*numCols + 1); // string of all the characters in the map
  map[0] = '\0'; // initialize with null terminator
  char* line;
  for (int i = 0; (line = file_readLine(fp)) != NULL; i++) {
    for (int j = 0; j < numCols; j++) {
      char c = line[j];
      strncat(map, &c, 1);
    }
    free(line);
  }

  grid->map = map; // store this character string in map member
  // free(map);

  // DEBUGGING
  // printf("%s\n", map);
  // printf("%d\n%d\n", grid->NR, grid->NC);

  fclose(fp);

  // for each character, create gridcell and put into grid array 
  for (int i = 0; i < strlen(map); i++) {
    gridcell_t* gridcell = gridcell_new(map[i], i % numCols, (int) ceil(i / numCols), 0, false);
    grid->gridarray[i] = gridcell;
  }
}

/* set a gridcell at a certain location x,y to a certain character c. See 'grid.h' for more info */
void grid_set(grid_t* grid, int x, int y, char c)
{
  if (grid == NULL || x < 0 || y < 0) {
    fprintf(stderr, "One or more grid_set args is null");
  }

 // calculate index by # of cols * y value + x value
  int idx = grid->NC * y + x;

  gridcell_set(grid->gridarray[idx], c);
  grid->map[idx] = c;
}

void grid_print(grid_t* grid)
{
  printf("%s", grid->map);
}

void grid_delete(grid_t* grid)
 {
  int arraySize = grid->NR * grid->NC;   // calculate size of gridarray

  for (int i = 0; i < arraySize; i++) {
    gridcell_delete(grid->gridarray[i]);
  }

  free(grid->map);
  free(grid->gridarray);
  free(grid);
 }
