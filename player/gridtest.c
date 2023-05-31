/*
 * gridtest.c - testing for grid
 * 
 * CS50 Nuggets Final Project
 * Team 17 - CecsC
 */


#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "grid.h"
#include "gridcell.h"
#include "player.h"

int main()
{
    grid_t* grid = grid_new();

    grid_load(grid, "../maps/visdemo.txt");
    // grid_generateGold(grid, 4, 10, 100);

    grid_print(grid);
    
    grid_delete(grid);

}