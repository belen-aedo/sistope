#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_COMANDOS 6
#define MAX_ARGUMENTOS 20
#define MAX_LONGITUD 1024

typedef struct {
    char *nombre;
    char *argumentos[MAX_ARGUMENTOS];
    int num_args;
} Comando;

typedef struct {
    Comando comandos[MAX_COMANDOS];
    int num_comandos;
    int pipes[MAX_COMANDOS - 1][2];
} Pipeline;

int parsear_linea(const char* linea, Pipeline* pipeline);
int ejecutar_pipeline(Pipeline* pipeline);
void liberar_pipeline(Pipeline* pipeline);

#endif