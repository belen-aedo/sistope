//Belen Aedo - 21.556.446-0 
//Jordán Arias - 21.317.055-4


#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <unistd.h>

/* Constantes */
#define MAX_CMDS 10      //maximo de comandos en el pipeline
#define MAX_ARGS 20      //maimo de argumentos por comando

/* Estructura para representar un comando */
typedef struct {
    char *argv[MAX_ARGS]; //argumentos para execvp() 
    int argc;             //cantidad de argumentos
} Command;


/**
 * Procesa una línea con pipes y separa los comandos y argumentos.
 * @param line Línea ingresada por el usuario
 * @param cmds Arreglo donde se almacenan los comandos procesados
 * @return Cantidad de comandos encontrados
 */
int procesarPipeline(char *line, Command cmds[]);

/**
 * Ejecuta un pipeline tipo Bash usando fork, pipe, dup2 y execvp.
 * @param cmds Arreglo de comandos a ejecutar
 * @param cont_comandos Cantidad total de comandos
 */
void ejecutarPipeline(Command cmds[], int cont_comandos);

#endif
