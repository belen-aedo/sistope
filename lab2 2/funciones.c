#include "funciones.h"
#include <ctype.h>

// Función auxiliar para duplicar string
char* duplicar_string(const char* src) {
    if (!src) return NULL;
    char* dst = malloc(strlen(src) + 1);
    if (dst) strcpy(dst, src);
    return dst;
}

// Función para parsear una línea de comando respetando comillas
int parsear_comando(const char* cmd_line, Comando* comando) {
    char copia[MAX_LONGITUD];
    strncpy(copia, cmd_line, MAX_LONGITUD - 1);
    copia[MAX_LONGITUD - 1] = '\0';
    
    comando->num_args = 0;
    memset(comando->argumentos, 0, sizeof(comando->argumentos));
    
    int i = 0;
    int en_comillas = 0;
    char comilla_char = 0;
    char buffer[MAX_LONGITUD] = {0};
    int buf_idx = 0;
    
    while (copia[i] != '\0') {
        if (copia[i] == '"' || copia[i] == '\'') {
            if (!en_comillas) {
                en_comillas = 1;
                comilla_char = copia[i];
            } else if (comilla_char == copia[i]) {
                en_comillas = 0;
            } else {
                // Comilla dentro de comillas
                buffer[buf_idx++] = copia[i];
            }
            i++;
        } else if (isspace(copia[i]) && !en_comillas) {
            // Fin de un argumento
            if (buf_idx > 0) {
                buffer[buf_idx] = '\0';
                comando->argumentos[comando->num_args] = duplicar_string(buffer);
                if (!comando->argumentos[comando->num_args]) {
                    return -1;
                }
                comando->num_args++;
                buf_idx = 0;
            }
            i++;
        } else {
            buffer[buf_idx++] = copia[i];
            i++;
        }
    }
    
    // Último argumento si queda algo en el buffer
    if (buf_idx > 0) {
        buffer[buf_idx] = '\0';
        comando->argumentos[comando->num_args] = duplicar_string(buffer);
        if (!comando->argumentos[comando->num_args]) {
            return -1;
        }
        comando->num_args++;
    }
    
    if (comando->num_args > 0) {
        comando->nombre = duplicar_string(comando->argumentos[0]);
        if (!comando->nombre) {
            return -1;
        }
    }
    
    return 0;
}

int parsear_linea(const char* linea, Pipeline* pipeline) {
    char copia[MAX_LONGITUD];
    strncpy(copia, linea, MAX_LONGITUD - 1);
    copia[MAX_LONGITUD - 1] = '\0';
    
    pipeline->num_comandos = 0;
    
    // Separar por pipes, respetando que puedan estar entre comillas
    char* inicio = copia;
    char* ptr = copia;
    int en_comillas = 0;
    char comilla_char = 0;
    
    while (*ptr != '\0') {
        if (*ptr == '"' || *ptr == '\'') {
            if (!en_comillas) {
                en_comillas = 1;
                comilla_char = *ptr;
            } else if (comilla_char == *ptr) {
                en_comillas = 0;
            }
        } else if (*ptr == '|' && !en_comillas) {
            // Encontramos un pipe que no está dentro de comillas
            *ptr = '\0';
            
            if (pipeline->num_comandos >= MAX_COMANDOS) {
                fprintf(stderr, "Error: Demasiados comandos (máximo %d)\n", MAX_COMANDOS);
                return -1;
            }
            
            // Parsear este comando
            if (parsear_comando(inicio, &pipeline->comandos[pipeline->num_comandos]) == -1) {
                fprintf(stderr, "Error parseando comando: %s\n", inicio);
                return -1;
            }
            
            pipeline->num_comandos++;
            inicio = ptr + 1;
        }
        ptr++;
    }
    
    // Parsear el último comando
    if (*inicio != '\0') {
        if (pipeline->num_comandos >= MAX_COMANDOS) {
            fprintf(stderr, "Error: Demasiados comandos (máximo %d)\n", MAX_COMANDOS);
            return -1;
        }
        
        if (parsear_comando(inicio, &pipeline->comandos[pipeline->num_comandos]) == -1) {
            fprintf(stderr, "Error parseando comando: %s\n", inicio);
            return -1;
        }
        
        pipeline->num_comandos++;
    }
    
    // DEBUG: Mostrar los comandos parseados
    printf("\n=== COMANDOS PARSEADOS ===\n");
    for (int i = 0; i < pipeline->num_comandos; i++) {
        printf("Comando %d: %s\n", i+1, pipeline->comandos[i].nombre);
        printf("  Argumentos (%d):", pipeline->comandos[i].num_args);
        for (int j = 0; j < pipeline->comandos[i].num_args; j++) {
            printf(" '%s'", pipeline->comandos[i].argumentos[j]);
        }
        printf("\n");
    }
    printf("==========================\n\n");
    
    return 0;
}

int ejecutar_pipeline(Pipeline* pipeline) {
    pid_t pids[MAX_COMANDOS];
    
    // Crear pipes
    for (int i = 0; i < pipeline->num_comandos - 1; i++) {
        if (pipe(pipeline->pipes[i]) == -1) {
            perror("Error al crear pipe");
            return -1;
        }
    }
    
    // Crear procesos hijos
    for (int i = 0; i < pipeline->num_comandos; i++) {
        pids[i] = fork();
        
        if (pids[i] == -1) {
            perror("Error en fork");
            return -1;
        }
        
        if (pids[i] == 0) { // Proceso hijo
            // Configurar redirecciones
            if (i > 0) {
                dup2(pipeline->pipes[i-1][0], STDIN_FILENO);
                close(pipeline->pipes[i-1][0]);
            }
            
            if (i < pipeline->num_comandos - 1) {
                dup2(pipeline->pipes[i][1], STDOUT_FILENO);
                close(pipeline->pipes[i][1]);
            }
            
            // Cerrar todos los pipes
            for (int j = 0; j < pipeline->num_comandos - 1; j++) {
                close(pipeline->pipes[j][0]);
                close(pipeline->pipes[j][1]);
            }
            
            // Preparar argumentos para execvp
            char* args[MAX_ARGUMENTOS + 1];
            for (int j = 0; j < pipeline->comandos[i].num_args; j++) {
                args[j] = pipeline->comandos[i].argumentos[j];
            }
            args[pipeline->comandos[i].num_args] = NULL;
            
            // DEBUG
            printf("[PID %d] Ejecutando: ", getpid());
            for (int j = 0; args[j] != NULL; j++) {
                printf("%s ", args[j]);
            }
            printf("\n");
            
            // Ejecutar comando
            execvp(args[0], args);
            
            // Si execvp falla
            perror("Error en execvp");
            exit(1);
        }
    }
    
    // Proceso padre: cerrar todos los pipes
    for (int i = 0; i < pipeline->num_comandos - 1; i++) {
        close(pipeline->pipes[i][0]);
        close(pipeline->pipes[i][1]);
    }
    
    // Esperar a todos los hijos
    for (int i = 0; i < pipeline->num_comandos; i++) {
        waitpid(pids[i], NULL, 0);
    }
    
    return 0;
}

void liberar_pipeline(Pipeline* pipeline) {
    for (int i = 0; i < pipeline->num_comandos; i++) {
        free(pipeline->comandos[i].nombre);
        for (int j = 0; j < pipeline->comandos[i].num_args; j++) {
            free(pipeline->comandos[i].argumentos[j]);
        }
    }
}