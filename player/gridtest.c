/*
 * gridtest.c - testing for grid
 * 
 * CS50 Nuggets Final Project
 * Team 17 - CecsC
 */


#include <stdlib.h>
#include <stdbool.h>
#include "grid.h"
#include "gridcell.h"

int main()
{
    grid_t* grid = grid_new();
    grid_load(grid, "../maps/challenge.txt");

    grid_print(grid);

    // visibility
    gridcell_t* g1 = grid_get(grid, 4, 1);
    gridcell_t* g2 = grid_get(grid, 4, 6);

    bool vis = grid_isVisible(grid, g1, g2);
    printf("Visibility: %d\n", vis);

    grid_delete(grid);

}