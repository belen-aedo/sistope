//Belen Aedo - 
//Jordán Arias - 21.317.055-4


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "funciones.h"

//Entradas: line es la linea completa ingresada por el usuario con los comandos separados por "|", cmds[] donde se almacenarán los comandos
//Salidas:Retorna la cantidad total de comandos encontrados
//Descripción: Separa la lsinea según "|" y divide cada comando en palabras,construyendo argv[] para que execvp() pueda ejecutarlos.
int procesarPipeline(char *line, Command cmds[]) {
    int cont_comandos = 0;  //contador de comandos encontrados en la linea

    //strtok separa la línea en partes usando el pipe ("|") como delimitador.
    //la primera llamada obtiene el primer comando completo.
    char *cmd = strtok(line, "|");

    //repetimos mientras existan comandos separados por "|"
    while (cmd != NULL && cont_comandos < MAX_CMDS) {

        //iniciamos el contador de argumentos del comando actual
        cmds[cont_comandos].argc = 0;

        //eliminamos espacios iniciales en caso de que el usuario escriba " | comando"
        while (*cmd == ' ') {
            cmd++;  
        }

        //strtok ahora separa por espacios dentro del comando actual
        char *arg = strtok(cmd, " \n\t");

        //repetimos mientras existan argumentos y no se supere el máximo permitido
        while (arg != NULL && cmds[cont_comandos].argc < MAX_ARGS - 1) {

            //guardamos un argumento en el arreglo argv[]
            cmds[cont_comandos].argv[cmds[cont_comandos].argc] = arg;

            //aumentamos el contador de argumentos
            cmds[cont_comandos].argc++;

            //buscamos el siguiente argumento
            arg = strtok(NULL, " \n\t");
        }

        //añadimos NULL al final de argv[]
        //execvp() exige que argv termine en NULL para saber cuando paran los argumentos
        cmds[cont_comandos].argv[cmds[cont_comandos].argc] = NULL;

        //hemos terminado de procesar este comando, pasamos al siguiente
        cont_comandos++;

        //buscamos la siguiente parte después del pipe "|"
        cmd = strtok(NULL, "|");
    }

    //retornamos la cantidad de comandos encontrados y procesados
    return cont_comandos;
}

//Entradas:cmds[] arreglo con todos los comandos a ejecutar en el pipeline, cont_comandos cantidad total de comandos a ejecutar
// Salidas:  No retorna nada, solo ejecuta el pipeline
// Descripción: Crea los pipes necesarios, genera un hijo por comando, redirige stdin/stdout, según corresponda y ejecuta execvp() para formar el pipeline tipo Bash.

void ejecutarPipeline(Command cmds[], int cont_comandos) {

    int pipes[cont_comandos - 1][2];

    //creamos los pipes necesarios
    for (int i = 0; i < cont_comandos - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }


    //crear un proceso por cada comando usando fork
    for(int i = 0; i < cont_comandos; i++) {

        pid_t pid = fork();

        if(pid == 0) {
            //HIJO
            //si no es el primer comando,recibe input del pipe anterior
            if(i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }

            //si no es el último comando, manda output al pipe siguiente
            if(i < cont_comandos - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            //Cerramos todos los pipes en el hijo
            for(int j = 0; j < cont_comandos - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            //ejecutamos el comando execvp
            execvp(cmds[i].argv[0], cmds[i].argv);

            perror("execvp");
            exit(1);
        }
    }

    //cerramos todos los pipes en el padre
    for(int i = 0; i < cont_comandos - 1; i++) {
{
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    //esperamos a que terminen todos los hijos
    for(int i = 0; i < cont_comandos; i++) {
        wait(NULL);
    }
}
}