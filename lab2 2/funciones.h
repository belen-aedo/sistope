//Belen Aedo - 21.556.446-0
//Jordán Arias - 21.317.055-4

#ifndef FUNCIONES_H
#define FUNCIONES_H

/*
 archivo: funciones.h
 descripcion:
 archivo de cabecera que define las estructuras, constantes
 y prototipos de funciones utilizadas para implementar
 el pipeline de ejecucion de scripts.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

/*
 constantes del sistema
 */
#define MAX_COMANDOS 6        // cantidad maxima de scripts en el pipeline
#define MAX_ARGUMENTOS 20     // cantidad maxima de argumentos por comando
#define MAX_LONGITUD 1024     // longitud maxima de una linea de entrada

/*
 estructura comando
 descripcion:
 almacena la informacion de un comando individual,
 incluyendo su nombre y sus argumentos.
 */
typedef struct {
    char *nombre;                          // nombre del comando o script
    char *argumentos[MAX_ARGUMENTOS + 1]; // lista de argumentos para execvp
    int num_args;                          // cantidad de argumentos
} Comando;

/*
 estructura pipeline
 descripcion:
 representa el pipeline completo, almacenando
 los comandos, la cantidad total y los pipes
 necesarios para la comunicacion entre procesos.
 */
typedef struct {
    Comando comandos[MAX_COMANDOS];          // arreglo de comandos
    int num_comandos;                        // cantidad de comandos en el pipeline
    int pipes[MAX_COMANDOS - 1][2];          // pipes para comunicacion entre comandos
} Pipeline;

/*
 entradas: linea completa ingresada por el usuario
 salidas: pipeline cargado con los comandos
 descripcion:
 separa la linea usando pipes y guarda cada comando
 dentro de la estructura pipeline.
 */
int separar_pipeline(const char *linea, Pipeline *pipeline);

/*
 entradas: pipeline con comandos cargados
 salidas: no retorna valor
 descripcion:
 crea procesos, configura pipes y ejecuta los scripts
 respetando el orden del pipeline.
 */
int ejecutar_pipeline(Pipeline *pipeline);

/*
 entradas: pipeline utilizado
 salidas: memoria liberada
 descripcion:
 libera la memoria reservada para los argumentos
 de cada comando del pipeline.
 */
void liberar_pipeline(Pipeline *pipeline);

#endif
