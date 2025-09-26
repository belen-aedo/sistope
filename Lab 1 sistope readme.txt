Integrantes: Belèn Aedo - Jordan Arias


Este laboratorio està compuesto por 6 script que se comunican mediante pipes de stdin/stdout
Los archivos de script cada uno tiene una funcionalidad:
* generator.sh: captura datos de procesos usando ps en intervalos regulares
* preprocess.sh: valida el formato y cambia deja los datos de forma ISO 8601
* filter.sh: filtra los procesos segun la cpu, memoria y comando dado
* transform.sh: anonimiza UID de usuario mediante hashing
* aggregate.sh: agrupa datos por comando y calcula el promedio
* report.sh: genera el reporte final, con los datos entregados anteriormente, en formato TSV/CSV


Para ejecutar correctamente el código, hay que ingresar mediante la terminal en donde se encuentren los archivos bash ./generator.sh -i number -t number | ./preprocess.sh | ./filter.sh -c number -m number -r "Process name" | ./transform.sh --anon-uid | ./aggregate.sh \ | ./report.sh -o reporte.tsv (o .csv). En donde number, puede ser cualquier número positivo. En donde primero se capturan los datos con generator, luego se le da un formato específico con preprocess, para filtrar por nombre gracias a filter, anonimizar los UID con transform, luego sacar datos porcentuales de los procesos filtrados, para finalmente generar un reporte .csv o . tsv.


Se debe verificar que todos los script tengan los permisos de ejecuciòn: 
chmod +x generator.sh
chmod +x preprocess.sh
chmod +x filter.sh
chmod +x transform.sh
chmod +x aggregate.sh
chmod +x report.sh


Ejemplos:


./generator.sh -i 1 -t 10 
| ./preprocess.sh --iso8601 
| ./filter.sh -c 3 -m 2 -r "^(gnome-shell)$" 
| ./transform.sh --anon-uid 
| ./aggregate.sh 
| ./report.sh -o reporte.tsv




./generator.sh -i 3 -t 9 
| ./preprocess.sh  --iso 8601
| ./filter.sh -c 0 -m 0 -r "^(brave|gnome-shell)$" 
| ./transform.sh --anon-uid 
| ./aggregate.sh \ 
| ./report.sh -o reporte.csv




./generator.sh -i 1 -t 15 \
| ./preprocess.sh \
| ./filter.sh -c 10 -m 5 -r ".*" \
| ./transform.sh --anon-uid \
| ./aggregate.sh \
| ./report.sh -o reporte.csv






Posibles errores:


Generator con entradas que no sean números enteros positivos mayores a 0
./generator.sh -i 0 -t 0


./generator.sh -i 0.5 -t 0.5


./generator.sh -i -4 -t -8


./generator.sh -i  -t 2


./generator.sh -i 2 -t 


./generator.sh -i  -t 




Filter sin parámetros
./generator.sh -i 2 -t 10 
| ./preprocess.sh 
| ./filter.sh -c  -m  -r  


Filtro con números negativos
./generator.sh -i 2 -t 10 
| ./preprocess.sh 
| ./filter.sh -c -5 -m 1  -r  


Filtro con letras en la posición de nùmeros
./generator.sh -i 2 -t 10 
| ./preprocess.sh 
| ./filter.sh -c x -m 1  -r  


Generar reporte sin el nombre correcto ni formato correcto
./generator.sh -i 2 -t 10 
| ./preprocess.sh --iso 8601
| ./filter.sh -c 0 -m 0 -r "gnome-shell-cal" 
| ./transform.sh --anon-uid 
| ./aggregate.sh \ 
| ./report.sh -o reporte.txt




./generator.sh -i 2 -t 10 
| ./preprocess.sh --iso 8601
| ./filter.sh -c 0 -m 0 -r "gnome-shell-cal" 
| ./transform.sh --anon-uid 
| ./aggregate.sh \ 
| ./report.sh -o entregable.tsv