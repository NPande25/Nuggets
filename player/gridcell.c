/* 
 * CS50 NUggets Project
 * Team 17 - CecsC
 * 
 * gridcell.c - CS50 'gridcell' module
 * The gridcell module defines each individual piece of the gridcell,
 * storing its character (wall, blank, gold, etc), its location, and
 * whether or not it can be seen 
 *
 * see gridcell.h for more information.
 *
 * CecsC 2023
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "mem.h"
#include "grid.h"
#include "gridcell.h"

/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct gridcell {
  char c;                       // actual makeup of cell
  int x;                        // x position in grid
  int y;                        //y position in grid
  int gold;                        //amount of gold in place, 0 if none
  bool show;                    //if cell can be seen by player or not
  //maybe add player variable
} gridcell_t;



gridcell_t* gridcell_new(char c, int x, int y, int gold, bool show) 
{
  gridcell_t* gridcell = mem_assert(malloc(sizeof(gridcell_t)), "Gridcell allocation error");
  
  gridcell->c = c;
  gridcell->x = x;
  gridcell->y = y;
  gridcell->gold = gold;
  gridcell->show = show;

  return gridcell;

}


void gridcell_set(gridcell_t* gridcell, char c)
{
  if (gridcell == NULL) {
    fprintf(stderr, "gridcell null in gridcell_set");
  }

  gridcell->c = c;
}


int gridcell_getX(gridcell_t* gridcell)
{
  if (gridcell == NULL) {
    fprintf(stderr, "gridcell null in gridcell_getX");
  }

  return gridcell->x;
}

int gridcell_getY(gridcell_t* gridcell)
{
  if (gridcell == NULL) {
    fprintf(stderr, "gridcell null in gridcell_getY");
  }

  return gridcell->y;
}


void gridcell_print(gridcell_t* gridcell)
{
  if (gridcell == NULL) {
    fprintf(stderr, "gridcell null in gridcell_print");
  }

  printf("%c\n", gridcell->c);
}


void gridcell_delete(gridcell_t* gridcell)
{
  if (gridcell == NULL) {
    fprintf(stderr, "Null gridcell to delete");
  } else {
    free(gridcell);
  }
}
