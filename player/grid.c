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


/* create new grid. See 'grid.h' for more info */
grid_t* grid_new() {
  // Allocate memory for the grid structure
  grid_t* grid = mem_assert(malloc(sizeof(grid_t)), "grid memory error");
  
  // Allocate memory for the rows of the gridarray
  // grid->gridarray = mem_assert(malloc(NR * NC * sizeof(gridcell_t*)), "gridarray memory error");

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

  // allocate for gridarray
  grid->gridarray = mem_assert(malloc(numRows * numCols * sizeof(gridcell_t*)), "gridarray memory error");


  // for each character, concatenate to end of string with strncat
  char* map = malloc((numRows)*(numCols+1) + 1); // string of all the characters in the map
  map[0] = '\0'; // initialize with null terminator
  char* line;
  char newLine = '\n';
  int totalIdx = 0;
  for (int i = 0; (line = file_readLine(fp)) != NULL; i++) {

    for (int j = 0; j < numCols; j++) {
      char c = line[j];
      // fprintf(stdout, "%c\n", c);
      strncat(map, &c, 1);   // add to map
      
      // create new gridcell at the approprate (x,y)
      // math: x = number of characters so far (totalIdx) $mod$ numCols
      // y = totalIdx / numCols, rounded down (by casting as int)
      gridcell_t* gridcell = gridcell_new(c, totalIdx % numCols, (int) (totalIdx / numCols), 0, false);
      grid->gridarray[totalIdx] = gridcell;
      if (c == '-' || c == '|' || c == '+' || c == '#') {
        gridcell_setWall(gridcell, true);
      } else {
        gridcell_setWall(gridcell, false);
      }

      totalIdx++;
    }

    strncat(map, &newLine, 1);
    free(line);
  }

  grid->map = map; // store this character string in map member

  fclose(fp);
}

/* set a gridcell at a certain location x,y to a certain character c. See 'grid.h' for more info */
void grid_set(grid_t* grid, int x, int y, char c)
{
  if (grid == NULL || x < 0 || y < 0) {
    fprintf(stderr, "One or more grid_set args is null");
  } else {

  // calculate index by # of cols * y value + x value
    int idx = grid->NC * y + x;

    gridcell_set(grid->gridarray[idx], c);
    grid->map[idx] = c;
  }
}

void grid_iterate(grid_t* grid, void* arg, void (*itemfunc)(void* arg, void* item))
{
  if (grid == NULL || (*itemfunc) == NULL) {
    fprintf(stderr, "One or more 'iterate' arguments is NULL");
  } else {
    for (int i = 0; i < (grid->NR * grid->NC); i++) {
      (*itemfunc)(arg, grid->gridarray[i]);
    }
  }

}


/* check if a terget gridcell is visible from the player gridcell. See 'grid.h' for more info */
bool grid_isVisible(grid_t* grid, gridcell_t* player, gridcell_t* target)
{
  if (grid == NULL || player == NULL || target == NULL) {
    fprintf(stderr, "One or more visibility arguments is NULL");
    return false;
  }


  int startX = gridcell_getX(player);       // 0
  int startY = gridcell_getY(player);       // 0

  int endX = gridcell_getX(target);         // 2
  int endY = gridcell_getY(target);         // 3

  int dx = endX - startX;                   // 2
  int dy = endY - startY;                   // 3

  if (dx == 0 && dy == 0) { // same point
    return true;
  }

  if (dx == 0 && dy > 0) { // vertical line, going down
    for (int y = startY + 1; y < endY; y++) {
      gridcell_t* g = grid_get(grid, startX, y);
      if (gridcell_isWall(g)) {
        return false;
      }
    }
    // if you go down the line and it doesn't hit a wall, return true
    return true;
  }

  if (dx == 0 && dy < 0) { // vertical line, going up
    for (int y = startY - 1; y > endY; y--) {
      gridcell_t* g = grid_get(grid, startX, y);
      if (gridcell_isWall(g)) {
        return false;
      }
    }
    // if you go up the line and it doesn't hit a wall, return true
    return true;
  }

  if (dy == 0 && dx > 0) { // horizontal line, going right
    for (int x = startX + 1; x < endX; x++) {
      gridcell_t* g = grid_get(grid, x, startY);
      if (gridcell_isWall(g)) {
        return false;
      }
    }
    // if you go right along the line and don't hit a wall, return true
    return true;
  }

  if (dy == 0 && dx < 0) { // horizontal line, going left
    for (int x = startX - 1; x > endX; x--) {
      gridcell_t* g = grid_get(grid, x, startY);
      if (gridcell_isWall(g)) {
        return false;
      }
    }
    // if you go left along the line and don't hit a wall, return true
    return true;
  }

  int incrX;
  int incrY;
  if (dx > 0) { // x values move left
    incrX = 1;
  } else {      // moving right
    incrX = -1;
  }

  if (dy > 0) { // y values moving down
    incrY = 1;
  } else {      // moving up
    incrY = -1;
  }

  // how each value will be incremented in the loops
  double stepX = (double)dx / dy;
  double stepY = (double)dy / dx;

  // LOOP 1 - x VALUES: for each integer x value in between startx and endx (exclusive),
  // calculate (double) y value and check the cells above and below for isWall
  for (int x = startX + incrX; x != endX; x += incrX) { // if dx > 0 (moving right), then x++; if dx < 0, x--
    // calculate y
    double y = startY + (x - startX) * stepY;   // 3/2  ----> point = (1,3/2)

    // calculate the two points we need to check. Points are (x, underY) and (x, overY)
    int underY = floor(y);  // 1
    int overY = ceil(y);    // 2
    // printf("%f with stepY = %f: %d, %d\n", y, stepY, underY, overY);

    // calculate indices for these points
    int underIdx = grid->NC * underY + x;
    int overIdx = grid->NC * overY + x; 

    gridcell_t* underG = grid->gridarray[underIdx];
    gridcell_t* overG = grid->gridarray[overIdx];

    // check if both are wall
    if (gridcell_isWall(underG) && gridcell_isWall(overG)) {
      return false;
    }
  }

  // LOOP 2 - Y VALUES: for each integer y value in between startY and endY (exclusive),
  // calculate x value and check the cells left and right for isWall
  for (int y = startY + incrY; y != endY; y += incrY) {
    double x = startX + (y - startY) * stepX; // 2/3 -----> point = (2/3)

    // calculate two points (underX, y) and (overX, y)
    int underX = floor(x);  // 0
    int overX = ceil(x);   // 1

    // indices in gridarray
    int underIdy = grid->NC * y + underX;
    int overIdy = grid->NC * y + overX;

    gridcell_t* underg = grid->gridarray[underIdy];
    gridcell_t* overg = grid->gridarray[overIdy];


    if (gridcell_isWall(underg) && gridcell_isWall(overg)) {
      return false;
    }
  }

  return true; // if walls have not been hit in any loop, it's visible

}

/*
 * in the future, second argument will be player_t* player
 * then, instead of gridcell_t* player, we just start off with:
 * gridcell_t* g = grid_get(grid, player_get_x(player), player_get_y(player))
 */
char* grid_playerVisibility(grid_t* grid, gridcell_t* player)
{
  if (grid == NULL || player == NULL) {
    fprintf(stderr, "Null argument(s) in grid_playerVisibility");
    return NULL;
  }

  // gridcell_t* player = grid_get(grid, player_get_x(player), player_get_y(player))
  // printf("%d, %d\n", gridcell_getX(player), gridcell_getY(player));
  // printf("%s\n", grid->map);

  // NEED ???????
  int x = gridcell_getX(player);
  int y = gridcell_getY(player);

  char* vis = mem_assert(malloc(grid->NR * (grid->NC + 1) + 1), "Visibility string mem error");
  vis[0] = '\0'; // initialize with null character
  char newLine = '\n';
  char invisible = ' ';
  char at = '@';

  for (int i = 0; i < grid->NC * grid->NR; i++) {
    gridcell_t* g = grid->gridarray[i];
    bool show = grid_isVisible(grid, player, g); // check visibility

    char c = gridcell_getC(g);
    if (show) { // if visible, add to string
      if (i == grid->NC * y + x) {
        strncat(vis, &at, 1);
      } else {
        strncat(vis, &c, 1);
      }
      gridcell_setShow(g, true);
    } else {
      strncat(vis, &invisible, 1);
    }

    if (((i+1) % grid-> NC) == 0) { // if we're at the end of the row, add newLine char
      strncat(vis, &newLine, 1);
    }
  }
  return vis;
}

void grid_generateGold(grid_t* grid, int minPiles, int maxPiles)
{
  if (grid != NULL && minPiles > 0 && maxPiles > minPiles) {
    // get random integer
    int range = maxPiles - minPiles;
    int r = rand() * range + minPiles; // fits random number in between minPiles and maxPiles

    r = 4;

    for (int i = 0; i < r; i++) {
      int randInt; // random integer from 0 to grid->NC * grid->NR
      grid->
    }
  }
}

gridcell_t* grid_get(grid_t* grid, int x, int y)
{
  if (grid == NULL || x < 0 || y < 0) {
    fprintf(stderr, "Invalid arguments for grid_get");
    return NULL;
  }

  // calculate idx
  int idx = grid->NC * y + x;
  return grid->gridarray[idx];
}

void grid_print(grid_t* grid)
{
  printf("%s\n", grid->map);

  // checking if isWall works
  // for (int i = 0; i < grid->NR * grid->NC; i++) {
  //   printf("%c", gridcell_getC(grid->gridarray[i]));
  //   printf("\n");
  //   bool wall = gridcell_isWall(grid->gridarray[i]);
  //   printf("Wall: %d, At: (%2d, %2d), Char: %c\n", wall, gridcell_getX(grid->gridarray[i]), gridcell_getY(grid->gridarray[i]), gridcell_getC(grid->gridarray[i]));
  // }
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
