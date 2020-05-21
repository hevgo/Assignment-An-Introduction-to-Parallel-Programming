CC = gcc
CFLAGS = -g -Wall -fopenmp
#INC = 
SRC = omp_trap1.c
#DEF = -lpthread

all:
	$(CC) $(CFLAGS) -o ./run $(SRC)
two:
	$(CC) $(CFLAGS) -o ./run omp_trap2a.c
clean:
	rm -rf ./run