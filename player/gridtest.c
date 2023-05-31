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
    grid_t* small = grid_new();
    grid_t* main = grid_new();
    grid_t* ateam = grid_new();

    // loads the map file into the grid
    grid_load(small, "../maps/small.txt");
    grid_load(main, "../maps/main.txt");
    grid_load(ateam, "../maps/contrib21s/ateam.txt");

    // prints out the grid that was loaded in
    printf("Map 1: small\nMap 2: main\nMap 3: ateam, from contrib21s\n\n");
    grid_print(small);
    printf("\n-------------------------------------------------\n");
    grid_print(main);
    printf("\n-------------------------------------------------\n");
    grid_print(ateam);

    grid_delete(small);
    grid_delete(main);
    grid_delete(ateam);

}