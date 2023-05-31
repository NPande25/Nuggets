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
#include "mem.h"
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
  bool room;                    // is cell in a room?
  bool isWall;                  // is cell a wall?
} gridcell_t;



gridcell_t* gridcell_new(char c, int x, int y, int gold, bool show, bool room) 
{
  // check args
  if (x < 0 || y < 0 || gold < 0) {
    fprintf(stderr, "invalid gridcell_new parameters");
    return NULL;
  }

  gridcell_t* gridcell = mem_assert(malloc(sizeof(gridcell_t)), "Gridcell allocation error");
  
  gridcell->c = c;
  gridcell->x = x;
  gridcell->y = y;
  gridcell->gold = gold;
  gridcell->show = show;
  gridcell->room = room;

  return gridcell;
}


void gridcell_set(gridcell_t* gridcell, char c)
{
  if (gridcell == NULL) {
    fprintf(stderr, "gridcell null in gridcell_set");
  }

  gridcell->c = c;
}

char gridcell_getC(gridcell_t* gridcell) 
{
  if (gridcell == NULL) {
    return '\0';
  }

  return gridcell->c;
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
    return 0;
  }

  return gridcell->y;
}

void gridcell_setGold(gridcell_t* gridcell, int gold)
{
  if (gridcell == NULL) {
    fprintf(stderr, "gridcell null in gridcell_setGold");
  } else {
    gridcell->gold = gold;
  }
}

int gridcell_getGold(gridcell_t* gridcell)
{
  if (gridcell == NULL) {
    fprintf(stderr, "gridcell null in gridcell_getGold");
    return 0;
  }

  return gridcell->gold;
}

void gridcell_setWall(gridcell_t* gridcell, bool isWall)
{
  if (gridcell == NULL) {
    fprintf(stderr, "gridcell null in gridcell_getY");
  } else {
    gridcell->isWall = isWall;
  }
}

bool gridcell_isWall(gridcell_t* gridcell)
{
  if (gridcell == NULL) {
    fprintf(stderr, "gridcell null in gridcell_isWall");
    return false;
  }

  return gridcell->isWall;
}

void gridcell_setShow(gridcell_t* gridcell, bool show) {
  if (gridcell == NULL) {
    fprintf(stderr, "gridcell null in gridcell_isWall");
  } else {
    gridcell->show = show;
  }

}

void gridcell_setRoom(gridcell_t* gridcell, bool room)
{
  if (gridcell == NULL) {
    fprintf(stderr, "gridcell null in gridcell_setRoom");
  } else {
    gridcell->room = room;
  }
}

bool gridcell_getRoom(gridcell_t* gridcell)
{
  if (gridcell == NULL) {
    fprintf(stderr, "gridcell null in gridcell_getRoom");
    return false;
  } else {
    return gridcell->room;
  }
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
    fprintf(stderr, "Null gridcell to delete\n");
  } else {
    free(gridcell);
  }
}
