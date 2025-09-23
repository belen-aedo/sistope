#!/bin/bash

#
#Entradas: datos por stdin, -c cpu, -m memoria, -r expresion regular
#Salidas: datos filtrados
#Descripción: filtra procesos segun los criterios dados

#variables
cpu_min=""
mem_min=""
expre=""

# Leer argumentos
while [ $# -gt 0 ]; do
    case $1 in
        -c) cpu_min=$2; shift 2 ;; #eliminamos 2 elementos con el shift
        -m) mem_min=$2; shift 2 ;; #pasamos a los siguientes 2 y eliminamos
        -r) expre=$2; shift 2 ;; #pasamos a los siguientes 2 y eliminamos
        *) echo "El ingreso de los datos es: $0 [-c CPU] [-m MEM] [-r EXPRE]"; exit 1 ;;
    esac
done

#funcion la cual sirve para verificar si un numero es mayor o igual
#Entradas: $1 = numero1, $2 = numero2
#Salidas: 0 si num1 >= num2, 1 si no
#Descripcion: compara dos numeros decimales
es_mayor_igual() {
    local num1="$1"
    local num2="$2"
    
    # Usar awk para comparar decimales
    resultado=$(echo "$num1 $num2" | awk '{if ($1 >= $2) print "si"; else print "no"}')
    
    if [ "$resultado" = "si" ]; then
        return 0
    else
        return 1
    fi
}

#recorremos las lineas dadas
while read linea; do
    #se salta las lineas vacias
    if [ -z "$linea" ]; then
        continue
    fi
    
    #sacamos los elememtos y los asignamos a variables
    timestamp=$(echo "$linea" | cut -f1)
    pid=$(echo "$linea" | cut -f2)
    uid=$(echo "$linea" | cut -f3)
    comm=$(echo "$linea" | cut -f4)
    pcpu=$(echo "$linea" | cut -f5)
    pmem=$(echo "$linea" | cut -f6)
    
    #bandera para ver si pasa los filtros
    pasa_filtros=true
    
    #cpu minima
    if [ -n "$cpu_min" ]; then
        if ! es_mayor_igual "$pcpu" "$cpu_min"; then
            pasa_filtros=false
        fi
    fi
    
    #memoria minima
    if [ -n "$mem_min" ]; then
        if ! es_mayor_igual "$pmem" "$mem_min"; then
            pasa_filtros=false
        fi
    fi
    
    #expresion regular 
    if [ -n "$expre" ]; then
        if ! echo "$comm" | grep -q "$expre"; then
            pasa_filtros=false
        fi
    fi
    
    #si pasa todos los filtros anteriores se muestra la linea
    if [ "$pasa_filtros" = true ]; then
        echo "$linea"
    fi
done
