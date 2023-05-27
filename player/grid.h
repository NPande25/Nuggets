/*
* Team 17 - CecsC
* Nuggets Final Project
* COSC 50, 23S
* 
* grid.h - header file for grid.c
*
* grid module defines the grid on 
* which the game is played
*/



#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "file.h"
#include "mem.h"
#include "gridcell.h"

/*
* grid struct, including members:
*   gridcell_t** gridArray;
*   Int NR;
*   Int NC;
*/
typedef struct grid grid_t;

/******** grid_new **************
 * creates grid and allocates for grid, gridarray, and all gridcells within it
 * 
 * 
 */
grid_t* grid_new(int NR, int NC);

/*
* loads char map into grid initializing gridcells?
*/

/********** grid_load *********
 * Load a grid from a file specified by the path name. The file represents 
 * a grid where each character in the file corresponds to a cell in the grid.
 * 
 * Input:
 *    grid - Pointer to the grid structure to be populated with data from the file.
 *    pathName - Path name of the file to be read.
 * output:
 *    the grid->map and grid->gridarray members will be filled according to the 
 *    map file that's passed in
 * Note:
 *    The grid structure must be allocated before calling this function.
 *    The file should contain characters representing the grid, with each character corresponding to a cell in the grid.
 */
void grid_load(grid_t* grid, char* pathName);


/*********** grid_set **************
 * change the character at a certain location in the grid
 * inputs:
 *     grid - grid whose character we're changing
 *     x - x coordinate of location to change
 *     y - y coordinate
 *     c - character to change to
 * outputs:
 *     character is changed at the appropriate index
 *     in the gridarray of gridcells and the char* map
 * 
 */
void grid_set(grid_t* grid, int x, int y, char c);

void grid_print(grid_t* grid);


// char* grid_iterate(grid_t* grid, void* arg, (*itemfunc)());

// grid_generateGold(grid_t* grid)

// grid_updateGold(grid_t* grid)

// grid_getGold(grid_t* grid)

// grid_isVisible(grid_t* grid)


/********** grid_delete ************
 * Deletes the grid struct, its map and gridarray
 * Input:
 *     grid - grid to delete
 * output:
 *     deletes the grid, freeing all its memory
 */
void grid_delete(grid_t* grid );
