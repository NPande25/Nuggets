# CS50 Nuggets Project
# Team 17 - CecsC
#
# Makefile - player directory
#

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$(L) -I$(LL) 
CC = gcc
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all


# directories 
L = ../libcs50
LL = ../support

# libraries
LIB = $(L)/libcs50.a
LIB1 = $(LL)/support.a
LIBOUT = player.a

# programs
PROG = gridtest
OBJS = grid.o

all: $(LIBOUT)

$(LIBOUT): gridcell.o grid.o player.o
		ar cr $(LIBOUT) $^


gridtest: gridtest.c grid.o gridcell.o $(LIB) $(LIB1)
	$(CC) $(CFLAGS) $^ -lm -o $@

visibilitytest: visibilitytest.c grid.o gridcell.o $(LIB) $(LIB1)
	$(CC) $(CFLAGS) $^ -lm -o $@

test: gridtest visibilitytest
	$(VALGRIND) ./gridtest
	$(VALGRIND) ./visibilitytest


gridtest.o: gridtest.c grid.h

grid.o: grid.h

gridcell.o: gridcell.h

player.o: player.h

clean:
	rm -f gridtest
	rm -f visibilitytest
	rm -f *.o
	rm -f $(LIBOUT)
