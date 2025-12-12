#include "funciones.h"
#include <sys/stat.h>

// Función auxiliar para limpiar espacios
char* trim(char* str) {
    while (*str == ' ' || *str == '\t' || *str == '\n') str++;
    
    if (*str == 0) return str;
    
    char* end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n')) end--;
    *(end + 1) = 0;
    
    return str;
}

// Función para duplicar string (strdup no siempre está disponible)
char* duplicar_string(const char* src) {
    if (!src) return NULL;
    char* dst = malloc(strlen(src) + 1);
    if (dst) strcpy(dst, src);
    return dst;
}

int parsear_linea(const char* linea, Pipeline* pipeline) {
    char copia[MAX_LONGITUD];
    strncpy(copia, linea, MAX_LONGITUD - 1);
    copia[MAX_LONGITUD - 1] = 0;
    
    // Contar comandos por pipes
    int count = 1;
    for (int i = 0; copia[i]; i++) {
        if (copia[i] == '|') count++;
    }
    
    if (count > MAX_COMANDOS) {
        fprintf(stderr, "Error: Demasiados comandos (max %d)\n", MAX_COMANDOS);
        return -1;
    }
    
    pipeline->num_comandos = 0;
    char* token = strtok(copia, "|");
    
    while (token && pipeline->num_comandos < count) {
        char* cmd = trim(token);
        if (!*cmd) {
            token = strtok(NULL, "|");
            continue;
        }
        
        // Separar comando y argumentos
        Comando* comando = &pipeline->comandos[pipeline->num_comandos];
        comando->num_args = 0;
        
        char* arg = strtok(cmd, " \t");
        while (arg && comando->num_args < MAX_ARGUMENTOS) {
            comando->argumentos[comando->num_args] = duplicar_string(arg);
            comando->num_args++;
            arg = strtok(NULL, " \t");
        }
        
        if (comando->num_args > 0) {
            comando->nombre = duplicar_string(comando->argumentos[0]);
        }
        
        pipeline->num_comandos++;
        token = strtok(NULL, "|");
    }
    
    return 0;
}

int ejecutar_pipeline(Pipeline* pipeline) {
    pid_t pids[MAX_COMANDOS];
    
    // Crear pipes
    for (int i = 0; i < pipeline->num_comandos - 1; i++) {
        if (pipe(pipeline->pipes[i]) == -1) {
            perror("pipe");
            return -1;
        }
    }
    
    // Crear procesos hijos
    for (int i = 0; i < pipeline->num_comandos; i++) {
        pids[i] = fork();
        
        if (pids[i] == -1) {
            perror("fork");
            return -1;
        }
        
        if (pids[i] == 0) { // Proceso hijo
            // Configurar redirecciones
            if (i > 0) {
                dup2(pipeline->pipes[i-1][0], STDIN_FILENO);
            }
            
            if (i < pipeline->num_comandos - 1) {
                dup2(pipeline->pipes[i][1], STDOUT_FILENO);
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
            
            // Ejecutar comando
            execvp(args[0], args);
            
            // Si execvp falla
            fprintf(stderr, "Error ejecutando: %s\n", args[0]);
            perror("execvp");
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