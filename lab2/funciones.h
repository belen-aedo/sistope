//Belen Aedo - 
//Jordán Arias - 21.317.055-4

#include "funciones.h"

// Entradas: argumentos, y cantidad de argumentos del main
// Salidas: estructura con scripts y flags
// Descripción: procesa la línea de comandos
void procesarEntrada(int argc, char *argv[]);

// Entradas: nombre del script, flags asociadas
// Salidas: vacio
// Descripción: ejecuta un script usando exec()
void ejecutarScript(char *script, char **flags);

// Entradas: cantidad de pipes a crear
// Salidas: arreglo de pipes
// Descripción: crea N pipes para el pipeline
int crearPipes(int pipes[][2], int n);