/* 
 * player.c - CS50 'player' module
 *
 * see player.h for more information.
 *
 * CecsC, 2023
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grid.h"

/**************** global types ****************/
typedef struct player {
  grid_t* grid;       // personal map of what they can see
  char c;               //what character they are
  int score;            //current score
  int x;                //location
  int y;                //location
  bool active;          //still in or has quit?
} player_t;

player_t* player_new(char c, char* map) {                               //note: should they get a grid loaded in?

    player_t* player = mem_mallocc(sizeof(player_t));

    player->grid = grid_new(NR, NC)                                     //note: should these get passed in or dimensions inferred?
    grid_load(map);
}