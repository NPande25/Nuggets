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
PROG = gridtest
OBJS = grid.o

gridtest: gridtest.c grid.o gridcell.o $(LIB) $(LIB1)
	$(CC) $(CFLAGS) $^ -lm -o $@

gridtest.o: gridtest.c grid.h

grid.o: grid.h

gridcell.o: gridcell.h

clean:
	rm -f gridtest
	rm -f *.o