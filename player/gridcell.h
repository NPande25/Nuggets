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
typedef struct gridcell gridcell_t;

// functions
gridcell_t* gridcell_new(char c, int x, int y, int gold, bool show);
void gridcell_set(gridcell_t* gridcell, char c);
int gridcell_getX(gridcell_t* gridcell);
int gridcell_getY(gridcell_t* gridcell);
char gridcell_getC(gridcell_t* gridcell);
void gridcell_setWall(gridcell_t* gridcell, bool isWall);
bool gridcell_isWall(gridcell_t* gridcell);
void gridcell_print(gridcell_t* gridcell);
void gridcell_delete(gridcell_t* gridcell);


