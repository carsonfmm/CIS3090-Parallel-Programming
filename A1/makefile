CC = gcc
CFLAGS = -std=gnu99 -Wpedantic

make all: gol_data gol_task

gol_data.o: gol_data.c
	$(CC) $(CFLAGS) -c -lpthread gol_data.c -o gol_data.o

gol_data: gol_data.o
	$(CC) $(CFLAGS) -o gol_data -lpthread gol_data.o

gol_task.o: gol_task.c
	$(CC) $(CFLAGS) -c -lpthread gol_task.c -o gol_task.o

gol_task: gol_task.o
	$(CC) $(CFLAGS) -o gol_task -lpthread gol_task.o

clean:
	rm gol_data gol_task *.o