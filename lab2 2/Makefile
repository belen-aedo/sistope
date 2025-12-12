# Makefile para Laboratorio 2 - Sistemas Operativos
#Belen Aedo - 21.556.446-0·

#Jordán Arias - 21.317.055-4

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
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

run: $(TARGET)
	chmod +x *.sh 2>/dev/null || true
	./$(TARGET) "./generator.sh -i 1 -t 2 | ./preprocess.sh | ./filter.sh -c 1.0 | ./aggregate.sh | ./report.sh -o reporte.tsv"

.PHONY: all clean run