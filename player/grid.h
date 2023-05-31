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
#include <unistd.h>
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
 * inputs:
 *     NR - number of rows
 *     NC - number of cols UPDATE OSINVOSIFLBNVOAIDBNOADGINBAOIDGB
 * output:
 *     grid struct that was created
 * notes:
 *     caller must later free the grid struct via grid_delete()
 */
grid_t* grid_new();

char* grid_get_map(grid_t* grid);

/******* grid_get_NR ******
 * get number of rows
 * input: grid of interest
 */
int grid_get_NR(grid_t* grid);

/******* grid_get_NC ******
 * get number of cols
 * input: grid of interest
 */
int grid_get_NC(grid_t* grid);

/******* grid_get_gridarray ******
 * get a gridcell from the gridarray
 * input: 
 *     grid of interest
 *     index at which the gridcell is
 */
gridcell_t* grid_get_gridarray(grid_t* grid, int idx);




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
 *    The file is assumed to be a valid rectangular map
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

/******** grid_get *******
 * gets a gridcell in the grid at a certain (x,y) location
 * 
 * Inputs:
 *     grid - grid in which we are getting a gridcell
 *     x - x position of gridcell to get
 *     y - y position
 * outputs:
 *     pointer to the gridcell at the corresponding location
 */
gridcell_t* grid_get(grid_t* grid, int x, int y);

/******** grid_print *********
 * prints the char* map of the grid
 * used for debugging
 */
void grid_print(grid_t* grid);

/*********** grid_update_map ***********
 * recreates the map string after it has changed
 * 
 * input:
 *     grid - grid whose string we are working with
 * output:
 *     Old grid->map freed. New string stored into grid->map.
 */
void grid_update_map(grid_t* grid);


/******** grid_iterate *********
 * iterate over all the gridcells in the grid
 * 
 * inputs: 
 *     grid - grid over which we're iterating
 *     arg - pointer to an object passed in, if necessary
 *     (*itemfunc) - function that acts on the gridcells
 * outputs:
 *     the itemfunc function is performed on every gridcell, which are modified in the grid
 */
void grid_iterate(grid_t* grid, void* arg, void (*itemfunc)(void* arg, void* item));




/********** grid_isVisible **************
 * determine if a target gridcell is visible from a player gridcell.
 * 
 * inputs:
 *     grid - grid in which we are determining visibility. Need this to check other gridcells in between.
 *     player - gridcell where the player is
 *     target - target gridcell. Is the target gridcell visible from the player?
 * outputs:
 *     bool - true if it's visible. False if not.
 * notes:
 *     pseudocode for this function is in IMPLEMENTATION.md and is not repeated here
 *     function is memory-neutral
 */
bool grid_isVisible(grid_t* grid, gridcell_t* player, gridcell_t* target);




/********* grid_generateGold ***********
 * create a random number of gold piles in a grid, between minPiles and maxPiles
 * 
 * inputs:
 *     grid - grid into which we're putting gold
 *     minPiles - minimum number of piles for randomization
 *     maxPiles - maximum number of piles
 *     goldTotal - int value for the total amount of gold between all piles
 * outputs:
 *     gridcell->gold value is increased for the cells that are randomly selected
 *
 */
void grid_generateGold(grid_t* grid, int minPiles, int maxPiles, int goldTotal);


// grid_updateGold(grid_t* grid)

// grid_getGold(grid_t* grid)


/********** grid_delete ************
 * Deletes the grid struct, its map and gridarray
 * Input:
 *     grid - grid to delete
 * output:
 *     deletes the grid, freeing all its memory
 */
void grid_delete(grid_t* grid );
