/*
 * visibilitytest.c - testing for visibility in the grid module
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
    grid_t* visdemo = grid_new();
    grid_t* visdemo_to_show = grid_new();

    grid_load(visdemo, "../maps/visdemo.txt");
    grid_load(visdemo_to_show, "../maps/visdemo.txt");

    // create gridcells to test visibility between
    gridcell_t* A = gridcell_new('.', 4, 1, 0, false, false);
    gridcell_t* g1 = gridcell_new('.', 18, 4, 0, false, false);
    gridcell_t* g2 = gridcell_new('.', 6, 7, 0, false, false);
    gridcell_t* g3 = gridcell_new('.', 21, 7, 0, false, false);

    // change characters on the show map so that we can see
    grid_set(visdemo_to_show, 4, 1, 'A');
    grid_set(visdemo_to_show, 23, 4, '1');
    grid_set(visdemo_to_show, 6, 7, '2');
    grid_set(visdemo_to_show, 21, 7, '3');


    grid_print(visdemo_to_show);

    // check visibility between these points
    bool vis1 = grid_isVisible(visdemo, A, g1);
    bool vis2 = grid_isVisible(visdemo, A, g2);
    bool vis3 = grid_isVisible(visdemo, A, g3);
    bool vis4 = grid_isVisible(visdemo, g1, g3);

    printf("1 Visible from A: %d\n", vis1);
    printf("2 Visible from A: %d\n", vis2);
    printf("3 Visible from A: %d\n", vis3);
    printf("3 Visible from 1: %d\n", vis4);

    // clean up
    gridcell_delete(A);
    gridcell_delete(g1);
    gridcell_delete(g2);
    gridcell_delete(g3);
    grid_delete(visdemo_to_show);
    grid_delete(visdemo);
}