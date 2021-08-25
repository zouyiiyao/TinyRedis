CC = gcc
CCFLAGS = -Wall -std=c99

RM = rm
RMFLAGS = -rf

all: zskiplist_test.c zmalloc.c zskiplist.c
	$(CC) $(CCFLAGS) zskiplist_test.c zmalloc.c zskiplist.c -o zskiplist_test

clean:
	$(RM) $(RMFLAGS) *.o
