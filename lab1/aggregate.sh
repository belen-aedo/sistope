#!/bin/bash
# Script: aggregate.sh
# Entradas: datos por stdin (timestamp, pid, uid, comm, pcpu, pmem)
# Salidas: datos agregados por comando (comm, nproc, cpu_avg, cpu_max, mem_avg, mem_max)
# Descripción: agrupa procesos por nombre de comando y calcula estadísticas

#se procesan los datos con getopts
while getopts "h" opt; do #lee los argumentos uno por uno y los asigna a una variable
    case $opt in
        h) echo "Uso: $0" >&2; echo "se leen los datos y los agrupa" >&2; exit 0 ;;
        *) echo "La opcion es invalida: -$OPTARG" >&2; exit 1 ;;
    esac
done

#crearemos un archivo temporal
temp_file="/tmp/aggregate_$$"
lineas_procesadas=0

#validaremos las lineqs entregadas
while read linea; do
    #se salta lineas vacías
    if [ -z "$linea" ]; then
        continue
    fi
    
    lineas_procesadas=$((lineas_procesadas + 1))
    
    #extraemos la CPU y memoria para validarla
    cpu=$(echo "$linea" | cut -d$'\t' -f5)
    mem=$(echo "$linea" | cut -d$'\t' -f6)
    
    #verificamos que la cpu sea un numero valido y no negativo
    if ! echo "$cpu" | grep -q '^[0-9]\+\(\.[0-9]\+\)\?$'; then
        continue
    fi
    
    #verificamos que la mem sea un numero valido y no negativo
    if ! echo "$mem" | grep -q '^[0-9]\+\(\.[0-9]\+\)\?$'; then
        continue
    fi
    
   
    #extraemos la linea de datos
    aux=$(echo "$linea" | cut -d$'\t' -f4)
    
    #guardamos los datos validos
    echo "$aux $cpu $mem" >> "$temp_file"
done

#verficamos que se entregaron datos y no era 0
if [ $lineas_procesadas -eq 0 ]; then
    echo "No se recibieron datos" >&2
    rm -f "$temp_file"
    exit 1
fi

#verificamos que los datos q nos entregaron, tenga datos para procesar
if [ ! -s "$temp_file" ]; then
    echo "No hay datos validos para procesar" >&2
    rm -f "$temp_file"
    exit 1
fi

#imprimir encabezado
echo -e "command\tnproc\tcpu_avg\tcpu_max\tmem_avg\tmem_max"

#usamos el awk para hacer calculos, dado que este comando lee linea por linea y nos separa los valores por columna, lo divide por espacios o tabulaciones
#en caso que colaramos el $0 muestra la linea completa
#tambien se usara el LC_NUMERIC=C , para evitar errores y siempre use . en vez de , 
LC_NUMERIC=C awk '{
    cmd = $1 #asignamos la cmd, que es el nombre del proceso
    cpu = $2 #asignamos la cpu
    mem = $3 #asignamos la memoria
    
    #contamos los procesos de la cmd, cada vez que aparezca le suma
    count[cmd]++
    
    #suma de la cpu
    cpu_sum[cmd] += cpu #guarda la suma total de la cpu de el proceso
    
    #si la cpu es mayor a la cpu_max , se reemplaza el valor de cpu_max
    #o si es la primera vez de este proceso se guarda igual
    if (cpu > cpu_max[cmd] || count[cmd] == 1) cpu_max[cmd] = cpu 
    
    #suma de la memoria  
    mem_sum[cmd] += mem
    
    #si la mem es mayor que la mem_max se reemplaza, por mem
    #o si es la primera vez de este proceso se guarda igual
    if (mem > mem_max[cmd] || count[cmd] == 1) mem_max[cmd] = mem
}
#el end lo usamos para ejecutar instrucciones solo 1 vez, despues que ya se hayan leido todas las lineas de procesos
END {
    for (cmd in count) {  #recorremos cada proceso de cmd que aparecio 
        cpu_avg = cpu_sum[cmd] / count[cmd] #calculamos el promedio de la cpu dividiendo por las veces que aparecio
        mem_avg = mem_sum[cmd] / count[cmd] #calculamos el promedio de la mem dividiendo por las veces que aparecio
        #imprimimos el resultado 
        #%s texto , %d numero entero, %.1f numero flotante
        printf "%s\t%d\t%.1f\t%.1f\t%.1f\t%.1f\n", cmd, count[cmd], cpu_avg, cpu_max[cmd], mem_avg, mem_max[cmd]
    }
}' "$temp_file"

#limpiamos el archivo temporal
rm -f "$temp_file"
