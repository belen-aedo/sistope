//Belen Aedo - 21.556.446-0
//Jordán Arias - 21.317.055-4
lab2.c            # programa principal
funciones.c       # separacion y ejecucion del pipeline
funciones.h       # estructuras, constantes 
Makefile          # archivo de compilacion

generator.sh      # script generador de datos
preprocess.sh    # script de preprocesamiento
filter.sh         # script de filtrado
aggregate.sh     # script de agregacion
report.sh        # script de generacion de reporte

compilacion: 

Para compilar el programa:
make

Esto genera el ejecutable:
lab2

Para limpiar los archivos compilados:
make clean

Despues de limpiar volvemos a ejecutar:
make


Antes de ejecutar el pipeline, es necesario otorgar permisos de ejecucion a los scripts:
chmod +x *.sh


ejemplos de ejecutar: 
ejemplo de ejecucion filtrando por firefox
./lab2 "./generator.sh -i 1 -t 2 | ./preprocess.sh | ./filter.sh -c 1.0 -r firefox | ./aggregate.sh | ./report.sh -o reporte.tsv"



las funciones usan :
fork() para crear procesos
pipe() para la comunicacion entre procesos
dup2() para redireccionar stdin y stdout
execvp() para ejecutar los scripts
wait() para sincronizar los procesos




Al finalizar la ejecucion del pipeline, se genera el archivo:
reporte.tsv o reporte.csv