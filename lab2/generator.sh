#!/bin/bash


#entradas: -i intervalo, -t tiempo total
#salidas: datos de procesos con el tiempo marcado
#descripción: se capturan los procesos que se realizan cada X segundos

#variables
intervalo=""
tiempo=""

##se leen los datos ingresados por el usuario i y t
while [ $# -gt 0 ]; do
    case $1 in #asignamos a intervalo el valor de la posicion 2
        -i) 
            # Verificar que hay un segundo argumento después de -i
            if [ -z "$2" ]; then
                echo "el parametro -i requiere de un valor"
                exit 1
            fi
            intervalo=$2; shift 2 ;; #con el shift eliminamos los primeros 2 argumentos
        -t) 
            # Verificar que hay un segundo argumento después de -t
            if [ -z "$2" ]; then
                echo "el parametro -t requiere de un valor"
                exit 1
            fi
            tiempo=$2; shift 2 ;; #el tiempo lo asignamos el valor de la posicion 2, ya que eliminamos los primeros ingresariamos al tiempo ejemplo -> -t 2
        *) echo "debe ingresar: $0 -i <intervalo> -t <tiempo>";
           exit 1 ;;
    esac
done

#se verifican que los datos esten ingresados
if [ -z "$intervalo" ] || [ -z "$tiempo" ]; then
    echo "-i y -t se deben ingresarse"
    exit 1 #salimos 
fi

#validar que intervalo sea un numero entero positivo
if [ -z "$intervalo" ]; then
    echo "el parametro -i no puede estar vacio"
    exit 1
fi
if ! [[ "$intervalo" =~ ^[0-9]+$ ]]; then
    echo "el parametro -i debe ser un numero entero positivo"
    exit 1
fi
if [ "$intervalo" -eq 0 ]; then
    echo "el parametro -i debe ser mayor que 0"
    exit 1
fi

#validar que tiempo sea un numero entero positivo 
if [ -z "$tiempo" ]; then
    echo "el parametro -t no puede estar vacio"
    exit 1
fi
if ! [[ "$tiempo" =~ ^[0-9]+$ ]]; then
    echo "el parametro -t debe ser un numero entero positivo"
    exit 1
fi
if [ "$tiempo" -eq 0 ]; then
    echo "el parametro -t debe ser mayor que 0"
    exit 1
fi


#haremos un ciclo para procesar por el tiempo dado 
tiempo_actual=0
while [ $tiempo_actual -lt $tiempo ]; do
    #se debe obtener el marcador de tiempo
    aux_tiempo=$(date "+%Y-%m-%dT%H:%M:%S%z")
    
    #ejecutamos el ps y le agregamos tiempo a cada linea
    ps -eo pid=,uid=,comm=,pcpu=,pmem= --sort=-%cpu | while read linea; do
        if [ -n "$linea" ]; then
            #usamos awk para quitar espacios que se encuentren al inicio y final y que estos esten de sobra, lo cual al separarlo por lo que es de columna , asignaremos cada uno de los valores
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
