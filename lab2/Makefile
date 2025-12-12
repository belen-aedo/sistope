# Belen Aedo - 
# Jordán Arias - 21.317.055-4

CC = gcc
CFLAGS = -Wall -Wextra -g
OBJ = lab2.o funciones.o

all: lab2

lab2: $(OBJ)
	$(CC) $(CFLAGS) -o lab2 $(OBJ)

lab2.o: lab2.c funciones.h
	$(CC) $(CFLAGS) -c lab2.c

funciones.o: funciones.c funciones.h
	$(CC) $(CFLAGS) -c funciones.c

clean:
	rm -f *.o lab2
