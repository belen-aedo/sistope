#!/bin/bash


#entradas: -i intervalo, -t tiempo total
#salidas: datos de procesos con el tiempo marcado
#descripción: se capturan los procesos que se realizan cada X segundos

#variables
intervalo=""
tiempo=""

#se leen los datos ingresados por el usuario i y t
while [ $# -gt 0 ]; do
    case $1 in #asignamos a intervalo el valor de la pocision 2
        -i) intervalo=$2; shift 2 ;; #con el shif eliminamos los primeros 2 argumentos
        -t) tiempo=$2; shift 2 ;; #el tiempo lo asignamos el valor de la posicion 2 , ya que eliminamos los primeros ingresariamos al tiempo ejemplo -> -t 2
        *) echo "debe ingresar: $0 -i INTERVALO -t TIEMPO"; exit 1 ;;
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
    # Obtener el marcador de tiempo
    aux_tiempo=$(date "+%Y-%m-%dT%H:%M:%S%z")
    
    #ejecutamos el ps y le agregamos tiempo a cada linea
    ps -eo pid=,uid=,comm=,pcpu=,pmem= --sort=-%cpu | while read linea; do
        if [ -n "$linea" ]; then
            #usamos awk para quitar espacios que se encuentren al inicio y final y que estos esten de sobra
            lineas=$(echo "$linea" | awk '{print $1 "\t" $2 "\t" $3 "\t" $(NF-1) "\t" $NF}')
            echo "$aux_tiempo	$lineas"
        fi
    done
    
    #sumamos los intervalos de tiempo para ver si el tiempo que ha pasado
    #es menor que el tiempo que debemos hacer
    tiempo_actual=$((tiempo_actual + intervalo))
    if [ $tiempo_actual -lt $tiempo ]; then
        sleep $intervalo #si es menor con el sleep espera el intervalo de tiempo que se le dio en el principio
    fi
done #termino de while
