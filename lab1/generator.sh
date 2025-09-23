#!/bin/bash


#Entradas: -i intervalo, -t tiempo total
#Salidas: datos de procesos con timestamp
#Descripción: se capturan los procesos cada X segundos

#variabls
intervalo=""
tiempo=""

#se leen los datos ingresados por el usuario i y t
while [ $# -gt 0 ]; do
    case $1 in #asignamos a intervalo el valor de la pocision 2
        -i) intervalo=$2; shift 2 ;; #con el shif eliminamos los primeros 2 argumentos
        -t) tiempo=$2; shift 2 ;; #el tiempo lo asignamos el valor de la posicion 2 , ya que eliminamos los primeros ingresariamos al tiempo ejemplo -> -t 2
        *) echo "Uso: $0 -i INTERVALO -t TIEMPO"; exit 1 ;;
    esac
done

#se verfiican que los datos esten ingresados
if [ -z "$intervalo" ] || [ -z "$tiempo" ]; then
    echo "-i y -t se deben ingresarse"
    exit 1 #salimos 
fi

#haremos un ciclo para procesar por el tiempo dado 
tiempo_actual=0
while [ $tiempo_actual -lt $tiempo ]; do
    # Obtener timestamp
    timestamp=$(date "+%Y-%m-%dT%H:%M:%S%z")
    
    #ejecutamos el ps y agregar timestamp a cada línea
    ps -eo pid=,uid=,comm=,pcpu=,pmem= --sort=-%cpu | while read linea; do
        if [ -n "$linea" ]; then
            #usamos awk para formatear 
            linea_limpia=$(echo "$linea" | awk '{print $1 "\t" $2 "\t" $3 "\t" $(NF-1) "\t" $NF}')
            echo "$timestamp	$linea_limpia"
        fi
    done
    
    #sumamos los intervalos de tiempo para ver si el tiempo que ha pasado
    #es menor que el tiempo que debemos hacer
    tiempo_actual=$((tiempo_actual + intervalo))
    if [ $tiempo_actual -lt $tiempo ]; then
        sleep $intervalo #si es menor con el sleep espera el intervalo de tiempo que se le dio en el principio
    fi
done #termino de while
