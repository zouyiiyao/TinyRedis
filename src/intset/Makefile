CC = gcc
CCFLAGS = -Wall -std=c99

RM = rm
RMFLAGS = -rf

all: intset_test.c zmalloc.c intset.c
	$(CC) $(CCFLAGS) intset_test.c zmalloc.c intset.c -o intset_test

clean:
	$(RM) $(RMFLAGS) *.o
