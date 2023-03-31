CC = gcc
CFLAGS = -std=gnu99 -Wpedantic

make all: a2encrypt a2decrypt

a2encrypt.o: a2encrypt.c
	$(CC) $(CFLAGS) -c a2encrypt.c -o a2encrypt.o

a2encrypt: a2encrypt.o
	$(CC) $(CFLAGS) -o a2encrypt a2encrypt.o

a2decrypt.o: a2decrypt.c
	mpicc -g -Wall -std=c99 -c a2decrypt.c -o a2decrypt.o

a2decrypt: a2decrypt.o
	mpicc -g -Wall -std=c99 -o a2decrypt a2decrypt.o

clean:
	rm a2encrypt a2decrypt *.o