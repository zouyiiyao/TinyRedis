CC = gcc
CCFLAGS = -Wall -std=c99

RM = rm
RMFLAGS = -rf

all: adlist_test.c zmalloc.c adlist.c
	$(CC) $(CCFLAGS) adlist_test.c zmalloc.c adlist.c -o adlist_test

clean:
	$(RM) $(RMFLAGS) *.o
