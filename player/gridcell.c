/* 
 * gridcell.c - CS50 'gridcell' module
 *
 * see gridcell.h for more information.
 *
 * CecsC 2023
 */


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
  int g;                        //amount of gold in place, 0 if none
  bool show;                    //if cell can be seen by player or not
} gridcell_t;


gridcell_t*