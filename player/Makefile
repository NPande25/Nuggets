# CS50 Nuggets Project
# Team 17 - CecsC
#
# Makefile - player directory
#

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$(L) -I$(LL) 
CC = gcc
MAKE = make

# directories 
L = ../libcs50
LL = ../support

# libraries
LIB = $(L)/libcs50.a
LIB1 = $(LL)/support.a

# programs
PROG = grid
PROG1 = gridcell
OBJS = grid.o
OBJS1 = gridcell.o


$(PROG): $(OBJS) $(LIB1) $(LIB)
	$(CC) $(CFLAGS) $^ -lm -o $@


$(PROG1): $(OBJS1) $(LIB1) $(LIB)
	# $(CC) $(CFLAGS) $^ -lm -o $@

gridcell.o: gridcell.h