#include "funciones.h"

int main(int argc, char* argv[]) {
    char linea[MAX_LONGITUD];
    Pipeline pipeline;
    
    printf("=== LABORATORIO 2 - PIPELINE DE PROCESOS ===\n");
    
    if (argc > 1) {
        // Usar argumento de línea de comandos
        strncpy(linea, argv[1], MAX_LONGITUD - 1);
        linea[MAX_LONGITUD - 1] = 0;
        printf("Comando: %s\n", linea);
    } else {
        // Leer de stdin
        printf("Ingrese pipeline: ");
        if (!fgets(linea, MAX_LONGITUD, stdin)) {
            perror("Error leyendo entrada");
            return 1;
        }
        linea[strcspn(linea, "\n")] = 0;
    }
    
    printf("\n");
    
    // Parsear línea
    if (parsear_linea(linea, &pipeline) == -1) {
        fprintf(stderr, "Error al parsear la línea\n");
        return 1;
    }
    
    if (pipeline.num_comandos == 0) {
        fprintf(stderr, "Error: No se encontraron comandos\n");
        return 1;
    }
    
    printf("Iniciando ejecución del pipeline...\n");
    printf("========================================\n");
    
    if (ejecutar_pipeline(&pipeline) == -1) {
        fprintf(stderr, "Error al ejecutar pipeline\n");
        liberar_pipeline(&pipeline);
        return 1;
    }
    
    printf("========================================\n");
    printf("Pipeline completado\n");
    
    liberar_pipeline(&pipeline);
    return 0;
}