//Belen Aedo - 21.556.446-0
//Jordán Arias - 21.317.055-4

Este laboratorio esta compuesto por 2 partes principales

La primera que ejecuta los scripts en un pipeline de procesos utilizando llamadas al sistema
* lab2.c           # programa principal
* funciones.c      # separacion y ejecucion del pipeline
* funciones.h      # estructuras, constantes 
* Makefile         # archivo de compilacion
 
Los script bash
* generator.sh     # script generador de datos
* preprocess.sh    # script de preprocesamiento
* filter.sh        # script de filtrado
* transform.sh     #script de anonimizacion
* aggregate.sh     # script de agregacion
* report.sh        # script de generacion de reporte





Para ejecutar correctamente el código, hay que ingresar mediante la terminal hacia la carpeta en la que se encuentren
los archivos, ejemplo:
cd Escritorio/21556446-0_21317055-4/sistope-master/lab2
Despues compilar el programa con make, lo que generara un ejecutable, posteriormente. En donde mediante las funciones
usan las funciones, fork() para crear procesos, pipe() para la comunicacion entre procesos, dup2() para redireccionar
stdin y stdout, execvp() para ejecutar los scripts, wait() para sincronizar los procesos.
Respecto a los script, primero capturan los datos con generator, luego se le da un formato específico con preprocess, para filtrar por 
nombre gracias a filter, anonimizar los UID con transform, luego sacar datos porcentuales de los procesos filtrados, 
para finalmente generar un reporte .csv o . tsv.




compilacion: 

Para compilar el programa:
make

Esto genera el ejecutable:
lab2

Antes de ejecutar el pipeline, es necesario otorgar permisos de ejecucion a los scripts:
chmod +x *.sh

ejemplos de ejecucion: 
ejemplo de ejecucion filtrando por firefox
./lab2 "./generator.sh -i 1 -t 2 | ./preprocess.sh | ./filter.sh -c 1.0 -r firefox | ./aggregate.sh | ./report.sh -o reporte.tsv"

Al finalizar la ejecucion del pipeline, se genera el archivo:
reporte.tsv o reporte.csv

Para limpiar los archivos compilados:
make clean