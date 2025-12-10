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

    printf("Ingresa el pipeline (ejemplo):\n");
    printf("./generator.sh -i 1 -t 5 | ./preprocess.sh | ./transform.sh --anon-uid\n\n");

    printf("> ");
    fgets(input, sizeof(input), stdin);

    Command cmds[MAX_CMDS];
    int cont_comandos = procesarPipeline(input, cmds);

    if(cont_comandos <= 0) {
        printf("No se encontraron comandos.\n");
        return 1;
    }

    ejecutarPipeline(cmds, cont_comandos);

    return 0;
}
