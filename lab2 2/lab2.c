//Belen Aedo - 21.556.446-0
//Jordán Arias - 21.317.055-4
#include "funciones.h"

/*
 programa principal que recibe una linea de comandos,
 separa el pipeline y ejecuta los scripts utilizando
 fork, pipe, dup2 y execvp.
 */

int main(int argc, char *argv[]) {

    char linea_comandos[MAX_LONGITUD];   // almacena la linea ingresada
    Pipeline pipeline;                   // estructura del pipeline

    /*
     entradas: argumentos de linea de comandos
     salidas: linea de texto con el pipeline
     descripcion:
     si el usuario entrega argumentos, se usa argv[1]
     si no, se solicita la linea por entrada estandar.
     */
    if (argc > 1) {

        // copiar la linea recibida por argumento
        strncpy(linea_comandos, argv[1], MAX_LONGITUD - 1);
        linea_comandos[MAX_LONGITUD - 1] = '\0';

    } else {

        // solicitar pipeline por teclado
        printf("ingrese pipeline: ");

        if (fgets(linea_comandos, MAX_LONGITUD, stdin) == NULL) {
            perror("error al leer entrada");
            return 1;
        }

        // eliminar salto de linea
        linea_comandos[strcspn(linea_comandos, "\n")] = '\0';
    }

    /*
     entradas: linea completa
     salidas: estructura pipeline cargada
     descripcion:
     se separa la linea por pipes y se cargan los comandos.
     */
    if (separar_pipeline(linea_comandos, &pipeline) == -1) {
        fprintf(stderr, "error al separar el pipeline\n");
        return 1;
    }

    // validar que exista al menos un comando
    if (pipeline.num_comandos == 0) {
        fprintf(stderr, "error: no se encontraron comandos\n");
        return 1;
    }

    /*
     entradas: pipeline cargado
     salidas: no retorna valor
     descripcion:
     se ejecuta el pipeline creando un proceso hijo
     por cada script llamado.
     */
    if (ejecutar_pipeline(&pipeline) == -1) {
        fprintf(stderr, "error al ejecutar el pipeline\n");
        liberar_pipeline(&pipeline);
        return 1;
    }

    /*
     liberar memoria utilizada
     */
    liberar_pipeline(&pipeline);

    return 0;
}
