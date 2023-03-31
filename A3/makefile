CC = gcc
CFLAGS = -std=gnu99 -Wpedantic

make all: maze mazep

maze.o: maze.c
	$(CC) $(CFLAGS) -fopenmp -c maze.c -o maze.o

maze: maze.o
	$(CC) $(CFLAGS) -fopenmp -o maze maze.c

mazep: maze.o
	$(CC) $(CFLAGS) -fopenmp -o mazep maze.c

clean:
	rm maze mazep *.o