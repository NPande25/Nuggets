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

int main()
{
    grid_t* grid = grid_new();
    grid_load(grid, "../maps/visdemo.txt");
    grid_generateGold(grid, 4, 10, 100);

    grid_print(grid);
    printf("\n---------------------------------------------------\n");

    // // visibility
    gridcell_t* g1 = grid_get(grid, 10, 2);
    // // gridcell_t* g2 = grid_get(grid, 4, 1);

    // // bool vis = grid_isVisible(grid, g1, g2);
    // // printf("Visibility: %d\n", vis);

    char* visMap = grid_playerVisibility(grid, g1);
    printf("%s\n", visMap);


    free(visMap);
    grid_delete(grid);

}