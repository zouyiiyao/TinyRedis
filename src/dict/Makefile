CC = gcc
CCFLAGS = -Wall -std=c99

RM = rm
RMFLAGS = -rf

all: dict_test.c zmalloc.c dict.c
	$(CC) $(CCFLAGS) dict_test.c zmalloc.c dict.c -o dict_test

clean:
	$(RM) $(RMFLAGS) *.o
