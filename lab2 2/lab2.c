/**
 * Laboratorio 2: Monitoreo y Procesamiento de Procesos
 * Programa principal
 * 
 * Integrantes: [NOMBRE1] (RUT1) - [NOMBRE2] (RUT2)
 */

#include "funciones.h"

int main(int argc, char* argv[]) {
    char linea[MAX_LONGITUD];
    Pipeline pipeline;
    
    if (argc > 1) {
        // Usar argumento de línea de comandos
        strncpy(linea, argv[1], MAX_LONGITUD - 1);
        linea[MAX_LONGITUD - 1] = 0;
    } else {
        // Leer de stdin
        printf("Ingrese pipeline: ");
        if (!fgets(linea, MAX_LONGITUD, stdin)) {
            perror("fgets");
            return 1;
        }
        linea[strcspn(linea, "\n")] = 0; // Remover newline
    }
    
    // Parsear línea
    if (parsear_linea(linea, &pipeline) == -1) {
        return 1;
    }
    
    if (pipeline.num_comandos == 0) {
        fprintf(stderr, "Error: No hay comandos\n");
        return 1;
    }
    
    // Ejecutar pipeline
    if (ejecutar_pipeline(&pipeline) == -1) {
        liberar_pipeline(&pipeline);
        return 1;
    }
    
    liberar_pipeline(&pipeline);
    return 0;
}