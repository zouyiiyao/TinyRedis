CC = gcc
CCFLAGS = -Wall -std=c99

RM = rm
RMFLAGS = -rf

all: sds_test.c zmalloc.c sds.c
	$(CC) $(CCFLAGS) sds_test.c zmalloc.c sds.c -o sds_test

clean:
	$(RM) $(RMFLAGS) *.o
