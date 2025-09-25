#!/bin/bash
#entradas:  -c cpu, -m memoria, -r expresion regular
#salidas: datos filtrados
#descripcion: filtraremos procesos segun los criterios dados

#variables
cpu_min=""
mem_min=""
expre=""

#leemos los argumentos dados por el usuario
while [ $# -gt 0 ]; do
    case $1 in
        -c) cpu_min=$2; shift 2 ;; #eliminamos 2 elementos con el shift
        -m) mem_min=$2; shift 2 ;; #pasamos a los siguientes 2 y eliminamos
        -r) expre=$2; shift 2 ;; #pasamos a los siguientes 2 y eliminamos
        *) echo "El ingreso de los datos es: $0 [-c CPU] [-m MEM] [-r EXPRE]" >&2; exit 1 ;;
    esac
done

#funcion la cual sirve para verificar si un numero es mayor o igual, y vemos si cumple con las condiciones
#entradas: $1 = numero1, $2 = numero2
#salidas: 0 si num1 >= num2, 1 si no
#descripcion: compara dos numeros decimales
comparacion() {
    local num1="$1"
    local num2="$2"
    
    #usamos el awk para comparar decimales
    resultado=$(echo "$num1 $num2" | awk '{if ($1 >= $2) print "si"; else print "no"}')
    
    if [ "$resultado" = "si" ]; then
        return 0
    else
        return 1
    fi
}

#recorremos las lineas dadas
while read linea; do
    #se salta las lineas que se encuentran vacias
    if [ -z "$linea" ]; then
        continue
    fi
    
    #sacamos los elementos y los asignamos a variables
    tiempo=$(echo "$linea" | cut -d$'\t' -f1)
    pid=$(echo "$linea" | cut -d$'\t' -f2)
    uid=$(echo "$linea" | cut -d$'\t' -f3)
    comm=$(echo "$linea" | cut -d$'\t' -f4)
    pcpu=$(echo "$linea" | cut -d$'\t' -f5)
    pmem=$(echo "$linea" | cut -d$'\t' -f6)
    
    #bandera para ver si pasa los filtros
    pasa_filtros=true
    
    #cpu minima
    if [ -n "$cpu_min" ]; then
        if ! comparacion "$pcpu" "$cpu_min"; then
            pasa_filtros=false
        fi
    fi
    
    #memoria minima
    if [ -n "$mem_min" ]; then
        if ! comparacion "$pmem" "$mem_min"; then
            pasa_filtros=false
        fi
    fi
    
    #expresion regular 
    if [ -n "$expre" ]; then
        if ! echo "$comm" | grep -E -q "$expre"; then
            pasa_filtros=false
        fi
    fi
    
    #si pasa todos los filtros anteriores se muestra la linea
    if [ "$pasa_filtros" = true ]; then
        echo "$linea"
    fi
done
