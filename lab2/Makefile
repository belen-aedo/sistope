# Belen Aedo - 
# Jordán Arias - 21.317.055-4

CC = gcc
CFLAGS = -Wall -g
OBJ = lab2.o funciones.o

all: lab2

lab2: $(OBJ)
	$(CC) $(CFLAGS) -o lab2 $(OBJ)

lab2.o: lab2.c funciones.h
funciones.o: funciones.c funciones.h
