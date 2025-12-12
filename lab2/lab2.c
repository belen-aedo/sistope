//Belen Aedo - 
//Jordán Arias - 21.317.055-4
// Entradas: línea de comandos al programa
// Salidas: ejecución del pipeline
// Descripción: crea los hijos,pipes y ejecuta los scripts
#include <stdio.h>
#include <string.h>
#include "funciones.h"

int main() {

    char input[1024];
    Command cmds[MAX_CMDS];

    
    while (1) {

        printf("lab2> ");
        fflush(stdout);

        //leer entrada del usuario
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        //procesar la línea de comandos
        int cont_comandos = procesarPipeline(input, cmds);

        if (cont_comandos > 0) {
            ejecutarPipeline(cmds, cont_comandos);
        }
    }

    return 0;
}