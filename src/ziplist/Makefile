CC = gcc
CCFLAGS = -Wall -std=c99

RM = rm
RMFLAGS = -rf

all: ziplist_test.c zmalloc.c ziplist.c
	$(CC) $(CCFLAGS) ziplist_test.c zmalloc.c ziplist.c -o ziplist_test

clean:
	$(RM) $(RMFLAGS) *.o
