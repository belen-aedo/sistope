//Belen Aedo - 21.556.446-0
//Jordán Arias - 21.317.055-4


#include "funciones.h"
#include <ctype.h>

/*
 entradas: texto 
 salidas: puntero a un nuevo string en memoria
 descripcion:
 esta funcion recibe un string y crea una copia usando memoria dinamica.
 se utiliza para guardar los argumentos de cada comando sin perderlos.
 */
char *copiar_string(const char *texto) {
    char *copia;

    // validar que el texto exista
    if (texto == NULL) {
        return NULL;
    }

    // reservar memoria para el nuevo string
    copia = malloc(strlen(texto) + 1);
    if (copia == NULL) {
        return NULL;
    }

    // copiar el contenido del string original
    strcpy(copia, texto);

    return copia;
}

/*
 entradas: linea de texto con un solo comando
 salidas: estructura comando cargada con nombre y argumentos
 descripcion:
 separa un comando en palabras (argumentos).
 respeta espacios y permite usar comillas simples o dobles.
 */
int separar_comando(const char *linea, Comando *cmd) {
    char linea_copia[MAX_LONGITUD];   // copia local de la linea
    char palabra[MAX_LONGITUD];       // buffer para cada argumento
    int i = 0;                        // indice de la linea
    int indice_palabra = 0;           // indice del buffer palabra
    int dentro_comillas = 0;          // indica si estamos dentro de comillas
    char tipo_comilla = 0;            // tipo de comilla usada

    // copiar la linea original para no modificarla
    strncpy(linea_copia, linea, MAX_LONGITUD - 1);
    linea_copia[MAX_LONGITUD - 1] = '\0';

    // inicializar cantidad de argumentos
    cmd->num_args = 0;

    // recorrer caracter por caracter la linea
    while (linea_copia[i] != '\0') {

        // detectar inicio de comillas
        if ((linea_copia[i] == '"' || linea_copia[i] == '\'') && !dentro_comillas) {
            dentro_comillas = 1;
            tipo_comilla = linea_copia[i];
        }
        // detectar cierre de comillas
        else if (linea_copia[i] == tipo_comilla && dentro_comillas) {
            dentro_comillas = 0;
        }
        // detectar separacion por espacios cuando no estamos en comillas
        else if (isspace(linea_copia[i]) && !dentro_comillas) {
            if (indice_palabra > 0) {
                // cerrar el argumento actual
                palabra[indice_palabra] = '\0';
                cmd->argumentos[cmd->num_args++] = copiar_string(palabra);
                indice_palabra = 0;
            }
        }
        // agregar caracter al argumento actual
        else {
            palabra[indice_palabra++] = linea_copia[i];
        }

        i++;
    }

    // guardar el ultimo argumento si existe
    if (indice_palabra > 0) {
        palabra[indice_palabra] = '\0';
        cmd->argumentos[cmd->num_args++] = copiar_string(palabra);
    }

    // marcar el final de los argumentos para execvp
    cmd->argumentos[cmd->num_args] = NULL;

    // el nombre del comando es el primer argumento
    cmd->nombre = cmd->argumentos[0];

    return 0;
}

/*
 entradas: linea completa ingresada por el usuario
 salidas: estructura pipeline cargada
 descripcion:
 separa la linea completa usando el caracter pipe '|'
 y guarda cada comando en el pipeline.
 */
int separar_pipeline(const char *linea, Pipeline *pipeline) {
    char linea_copia[MAX_LONGITUD];
    char *token;
    int indice = 0;

    // copiar la linea original
    strncpy(linea_copia, linea, MAX_LONGITUD - 1);
    linea_copia[MAX_LONGITUD - 1] = '\0';

    // inicializar cantidad de comandos
    pipeline->num_comandos = 0;

    // separar por pipes
    token = strtok(linea_copia, "|");
    while (token != NULL && pipeline->num_comandos < MAX_COMANDOS) {

        // separar cada comando en argumentos
        separar_comando(token, &pipeline->comandos[indice]);

        pipeline->num_comandos++;
        indice++;

        token = strtok(NULL, "|");
    }

    return 0;
}

/*
 entradas: pipeline con todos los comandos cargados
 salidas: no retorna valor
 descripcion:
 crea los pipes necesarios, genera procesos hijos
 y ejecuta cada script respetando el orden del pipeline.
 */
int ejecutar_pipeline(Pipeline *pipeline) {
    int i;
    pid_t pid;

    // crear los pipes entre comandos
    for (i = 0; i < pipeline->num_comandos - 1; i++) {
        pipe(pipeline->pipes[i]);
    }

    // crear un proceso hijo por cada comando
    for (i = 0; i < pipeline->num_comandos; i++) {

        pid = fork();

        if (pid == 0) {
            // proceso hijo

            // si no es el primer comando, leer del pipe anterior
            if (i > 0) {
                dup2(pipeline->pipes[i - 1][0], STDIN_FILENO);
            }

            // si no es el ultimo comando, escribir al pipe siguiente
            if (i < pipeline->num_comandos - 1) {
                dup2(pipeline->pipes[i][1], STDOUT_FILENO);
            }

            // cerrar todos los pipes en el hijo
            for (int j = 0; j < pipeline->num_comandos - 1; j++) {
                close(pipeline->pipes[j][0]);
                close(pipeline->pipes[j][1]);
            }

            // ejecutar el script correspondiente
            execvp(pipeline->comandos[i].argumentos[0],
                   pipeline->comandos[i].argumentos);

            // si execvp falla
            perror("error en execvp");
            exit(1);
        }
    }

    // proceso padre: cerrar todos los pipes
    for (i = 0; i < pipeline->num_comandos - 1; i++) {
        close(pipeline->pipes[i][0]);
        close(pipeline->pipes[i][1]);
    }

    // esperar a que terminen todos los procesos hijos
    for (i = 0; i < pipeline->num_comandos; i++) {
        wait(NULL);
    }

    return 0;
}

/*
 entradas: pipeline utilizado
 salidas: memoria liberada
 descripcion:
 libera la memoria reservada para los argumentos de cada comando.
 */
void liberar_pipeline(Pipeline *pipeline) {
    for (int i = 0; i < pipeline->num_comandos; i++) {
        for (int j = 0; j < pipeline->comandos[i].num_args; j++) {
            free(pipeline->comandos[i].argumentos[j]);
        }
    }
}
