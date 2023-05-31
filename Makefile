# Makefile for 'Nuggets'
#
# Team 17, May 2023

S = support
L = libcs50
P = player
OBJS = 
LIBS =
LLIBS = $S/support.a $P/player.a $L/libcs50.a -lm

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$S -I$P
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all: server client

# executable depends on object files
server: server.o $(LLIBS)
			 $(CC) $(CFLAGS) $^ $(LIBS) -o $@

client: client.o $(LLIBS)
			$(CC) $(CFLAGS) $^ $(LIBS) -lcurses -o $@ 

# object files depend on include files
server.o: $S/message.h $L/file.h $L/mem.h $P/player.h $P/grid.h $P/gridcell.h

client.o: $S/message.h

test: 

valgrind: 

clean:
	rm -f core
	rm -rf *~ *.o *.dSYM
	rm -f client
	rm -f server