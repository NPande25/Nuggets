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
#include "player.h"
#include "gridcell.h"

/*
* grid struct, including members:
*   gridcell_t** gridArray;
*   Int NR;
*   Int NC;
*/
typedef struct grid grid_t;

/*
* creates grid and allocates for grid, gridarray, and all gridcells within it
*/
grid_t* grid_new(int NR, int NC);

/*
* loads char map into grid initializing gridcells?
*/
grid_load( grid_t* grid, FILE* map);

grid_set( grid_t* grid, int x, int y, char c);

char* grid_iterate(grid_t* grid, void* arg, (*itemfunc)());

grid_generateGold(grid_t* grid)

grid_updateGold(grid_t* grid)

grid_getGold(grid_t* grid)

grid_isVisible(grid_t* grid)

grid_delete(grid_t* grid )
