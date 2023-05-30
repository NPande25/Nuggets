/*
 * CS50 Nuggets Project
 * Team 17 - CecsC
 * 
 * gridcell.h - header file for gridcell.c
 * 
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"

// Gridcell struct
/*
 *  char c;             // actual makeup of cell
 * int x;               // x position in grid
 * int y;               // y position in grid
 * int gold;            //amount of gold in place, 0 if none
 * bool show;           //if cell can be seen by player or not
 */
typedef struct gridcell gridcell_t;

// functions
/*********** gridcell_new ***********
 * create a new gridcell struct
 * 
 * Inputs:
 *     c - the character of the cell
 *     x - x position in the grid
 *     y - y position
 *     gold - the amount of gold there
 *     show - boolean: is this visible?
 *     room - boolean: is this cell in a room?
 * outputs:
 *     a new gridcell struct
 * notes:
 *     caller must later free this via gridcell_delete()
 */
gridcell_t* gridcell_new(char c, int x, int y, int gold, bool show, bool room);


/********* GETTER AND SETTER METHODS FOR GRIDCELLS **********/

/******* gridcell_set *******
 * set a gridcell to a corresponding character
 * 
 * inputs:
 *     gridcell - gridcell we're setting
 *     c - character we're setting it to
 */
void gridcell_set(gridcell_t* gridcell, char c);

/******** gridcell_getX *******
 * get x position of a gridcell
 * input:
 *     gridcell - gridcell of interest
 * output:
 *     int - the x position
 */
int gridcell_getX(gridcell_t* gridcell);

/******** gridcell_getY *******
 * get Y position of a gridcell
 * input:
 *     gridcell - gridcell of interest
 * output:
 *     int - the y position
 */
int gridcell_getY(gridcell_t* gridcell);

/******** gridcell_getC *******
 * get character of a gridcell
 * input:
 *     gridcell - gridcell of interest
 * output:
 *     char - the character at the gridcell
 */
char gridcell_getC(gridcell_t* gridcell);


/******** gridcell_setGold *******
 * set amount of gold in a gridcell
 * input:
 *     gridcell - gridcell of interest
 * output:
 *     changes gridcell->gold
 */
void gridcell_setGold(gridcell_t* gridcell, int gold);


/******** gridcell_getGold *******
 * get amount of gold in a gridcell
 * input:
 *     gridcell - gridcell of interest
 * output:
 *     int - the gold
 */
int gridcell_getGold(gridcell_t* gridcell);

/******** gridcell_setWall *******
 * set whether or not a certain gridcell is a wall
 * input:
 *     gridcell - gridcell of interest
 * output:
 *     gridcell->wall becomes what you set it too (isWall)
 */
void gridcell_setWall(gridcell_t* gridcell, bool isWall);

/******** gridcell_isWall *******
 * get bool if gridcell is a wall
 * input:
 *     gridcell - gridcell of interest
 * output:
 *     bool - true if it's a wall, false if not
 */
bool gridcell_isWall(gridcell_t* gridcell);

/******** gridcell_setShow *******
 * set show boolean for specific gridcell
 * input:
 *     gridcell - gridcell of interest
 * output:
 *     gridcell->show becomes what you set it too (show)
 */
void gridcell_setShow(gridcell_t* gridcell, bool show);

/******* gridcell_setRoom ********
 * set whether or not a gridcell is in a room
 * inputs:
 *     gridcell - gridcell of interest
 *     room - whether or not it's in a room
 */
void gridcell_setRoom(gridcell_t* gridcell, bool room);

/******* gridcell_setRoom ********
 * check whether or not a gridcell is in a room
 * inputs:
 *     gridcell - gridcell of interest
 * outputs:
 *     bool - whether or not it's in a room
 */
bool gridcell_getRoom(gridcell_t* gridcell);


/******** gridcell_print *******
 * print gridcell's character
 * input:
 *     gridcell - gridcell of interest
 * output:
 *     prints the character to stdout
 * notes:
 *     mostly used for debugging
 */
void gridcell_print(gridcell_t* gridcell);

/******** gridcell_delete *******
 * delete gridcell struct
 * input:
 *     gridcell - gridcell to delete
 * output:
 *     gridcell is completely freed, along with its members
 * note:
 *     gridcell must have been created with gridcell_new
 */
void gridcell_delete(gridcell_t* gridcell);


