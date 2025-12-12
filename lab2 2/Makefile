# Makefile para Laboratorio 2 - Sistemas Operativos
# Integrantes: [NOMBRE1] (RUT1) - [NOMBRE2] (RUT2)

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = 

TARGET = lab2
SOURCES = lab2.c funciones.c
HEADERS = funciones.h
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean