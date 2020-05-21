CC = mpicc
CFLAGS = -g -Wall
#INC = 
SRC = 3.2_ser.c
SRC2 = 3.2_par.c
DEF = -lm

all:
	$(CC) $(CFLAGS) -o ./run $(SRC2) $(DEF)
ser:
	$(CC) $(CFLAGS) -o ./run $(SRC) $(DEF)
par:
	$(CC) $(CFLAGS) -o ./run $(SRC2) $(DEF)

clean:
	rm -rf ./run