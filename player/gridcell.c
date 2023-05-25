/* 
 * gridcell.c - CS50 'gridcell' module
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
#include "grid.h"

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
  gridcell_t* gridcell = mem_malloc(sizeof(gridcell_t));
  
  gridcell->c = c;
  gridcell->x = x;
  gridcell->y = y;
  gridcell->gold = gold;
  gridcell->show = show;

}

void gridcell_delete(gridcell_t* gridcell)
{
  if (gridcell != NULL) {
    fprintf(stderr, "Null gridcell to delete");
  } else {
    free(gridcell);
  }
}

