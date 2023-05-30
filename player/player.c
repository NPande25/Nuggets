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
#include "mem.h"
#include "grid.h"
#include "message.h"

/**************** global types ****************/
typedef struct player {
  bool* boolGrid;       // personal map of what they can see
  char c;               //what character they are
  char* name;           //what they say their name is
  int score;            //current score
  int x;                //location
  int y;                //location
  bool active;          //still in or has quit?
  addr_t addr;    //address of client corresponding to player
} player_t;

// IMPLICIT DECLARATIONS
int player_get_x(player_t* player);
int player_get_y(player_t* player);



player_t* player_new(char c, char* name, addr_t addr, int NR, int NC) {                               //note: should they get a grid loaded in?

    player_t* player = mem_malloc(sizeof(player_t));

    player->boolGrid = mem_malloc(sizeof(bool) * (NR * NC));  
    for (int i = 0; i < (NR * NC); i++) {
      player->boolGrid[i] = false;
    }

    player->c = c;
    player->name = name;
    player->score = 0;
    player->x = 0;
    player->y = 0;
    player->active = true;                                                      //note should not start active yet?
    player->addr = addr;

    return player;
}


void player_delete(player_t* player) {

  if (player->boolGrid != NULL) {
    mem_free(player->boolGrid);
  }
  if (player != NULL) {
    mem_free(player);
  }
}


void player_playerVisibility(player_t* player, grid_t* grid)
{
  if (grid == NULL || player == NULL) {
    fprintf(stderr, "Null argument(s) in player_playerVisibility");
  } else {

    gridcell_t* g = grid_get(grid, player_get_x(player), player_get_y(player));


    for (int i = 0; i < grid_get_NC(grid) * grid_get_NR(grid); i++) {
      if (!player->boolGrid[i]) { // if it's false in the bool grid (we don't have to worry about true, can't turn to false)
        gridcell_t* g1 = grid_get_gridarray(grid, i);
        bool show = grid_isVisible(grid, g, g1); // check visibility
        player->boolGrid[i] = show; // set that in the boolGrid
      }
    }
  }
}

/***** GETTER / SETTER FUNCTIONS *****/
addr_t player_get_addr(player_t* player) {

  return player->addr;
}


bool player_get_boolGrid(player_t* player, int index) {

  if (player != NULL && player->boolGrid != NULL) {
    return player->boolGrid[index];
  } else {
    fprintf(stderr, "player or player boolgrid is null\n");
    return false;
  }
}

char player_get_c(player_t* player) {

  if (player != NULL) {
    return player->c;
  } else {
    fprintf(stderr, "player or player c is null\n");
    return '\0';
  }
}

char* player_get_name(player_t* player) {

  if (player != NULL && player->name != NULL) {
    return player->name;
  } else {
    fprintf(stderr, "player or player name is null\n");
    return NULL;
  }
}

int player_get_score(player_t* player) {

  if (player != NULL) {
    return player->score;
  } else {
    fprintf(stderr, "player or player score is null\n");
    return -1;
  }
}

int player_get_x(player_t* player) {

  if (player != NULL) {
    return player->x;
  } else {
    fprintf(stderr, "player or player x coord is null\n");
    return -1;
  }
}

int player_get_y(player_t* player) {

  if (player != NULL) {
    return player->y;
  } else {
    fprintf(stderr, "player or player y coord is null\n");
    return -1;
  }
}

bool player_is_active(player_t* player) {

  if (player != NULL) {
    return player->active;
  } else {
    fprintf(stderr, "player is null in _is_active");
    return false;
  }
}


void player_set_c(player_t* player, char c) {

  if (player != NULL) {
    player->c = c;
  }
}

void player_set_name(player_t* player, char* name) {

  if (player != NULL && name != NULL) {
    player->name = name;
  }
}


void player_set_score(player_t* player, int score) {

  if (player != NULL) {
    player->score = score;
  }
}

void player_set_boolGrid(player_t* player, int index, bool visible) {

  if (player != NULL) {
    player->boolGrid[index] = visible;
  }
}


void player_set_x(player_t* player, int x) {

  if (player != NULL) {
    player->x = x;
  }
}


void player_set_y(player_t* player, int y) {

  if (player != NULL) {
    player->y = y;
  }
}


void player_deactivate(player_t* player) {

  if (player->active == true) {
    player->active = false;
    return;
  }
}

char* player_get_string(player_t* player, grid_t* grid) {

  int totalCells = (grid_get_NC(grid)) * (grid_get_NR(grid));
  char* map = mem_malloc(sizeof(char) * (totalCells + grid_get_NR(grid)) + 1);
  int index = 0;
  for (int i; i < totalCells; i++) {

    gridcell_t* cell = (grid_get_gridarray(grid, i));                       //new getter needed
    char c = gridcell_getC(cell);

    //if cell has been seen
    if (player_get_boolGrid(player, i)) {

      if (c == '*') {

        bool stillVis = grid_isVisible(grid, grid_get(grid, player_get_x(player), player_get_y(player)), cell);
        if (stillVis) {
          map[index] = '*';
        } else {
          map[index] = '.';
        }
      } else {
        map[index] = c;
      }
    } else {
      map[index] = ' ';
    }
    index++;

    //at end of row?
    if (((i+1) % grid_get_NC(grid)) == 0) {
      map[index] = '\n';
      index++;
    }
  }

  map[index] = '\0';

  return map;

}

