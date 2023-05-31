#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "grid.h"
#include "message.h"

// GLOBAL STRUCTS
typedef struct player player_t;


player_t* player_new(char c, const char* name, const addr_t addr, int NR, int NC);

void player_delete(player_t* player);

/********** player_playerVisibility ***********
 * create a string with the visibility for a player, replacing
 * invisible cells with ' '
 * 
 * inputs:
 *     grid - grid of interest
 *     player - gridcell where the player is  ----- UPDATE THIS
 * output:
 *     boolGrid is updated accordingly
 */
void player_playerVisibility(player_t* player, grid_t* grid);

addr_t player_get_addr(player_t* player);

bool player_get_boolGrid(player_t* player, int index);

char player_get_c(player_t* player);

const char* player_get_name(player_t* player);

int player_get_score(player_t* player);

int player_get_x(player_t* player);

int player_get_y(player_t* player);

bool player_is_active(player_t* player);

void player_set_c(player_t* player, char c);

void player_set_name(player_t* player, char* name);

void player_set_score(player_t* player, int score);

void player_set_boolGrid(player_t* player, int index, bool visible);

void player_set_x(player_t* player, int x);

void player_set_y(player_t* player, int y);

void player_deactivate(player_t* player);

char* player_get_string(player_t* player, grid_t* grid);