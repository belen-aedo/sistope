#!/bin/bash


#Entradas: datos por stdin, --iso8601
#Salidas: datos validados
#Descripcion: ve que los datos de entrada sean validos

# Variables
iso8601=false

# vemos los argumentos
while [ $# -gt 0 ]; do
    case $1 in #si $1 que es el argumento actual
    #es --iso8601 activa la bandera de true
        --iso8601) iso8601=true; shift ;; #ya que se valido eliminamos este argumento. para que pase al siguiente
        *) echo "El uso correcto es -> $0 [--iso8601]"; exit 1 ;; #si tiene un argumento que no reconoce termina mostrando el mensaje como un error
    esac
done

#esta funcion servira para validar la linea
#Entradas: $1 = linea de dato
#Salidas: 0 si es valida, 1 si no
#Descripcipn: verifica que tenga 6 campos
validar_linea() {
    local linea="$1" #primer parametro que se le pasa
    local campos=$(echo "$linea" | tr '\t' ' ' | wc -w)
    
    if [ $campos -eq 6 ]; then #verificamos que tenga 6 elementos
        return 0 #return verdadero
    else
        return 1 #return falso , no tiene 6 campos
    fi
}

#procesamos todas las lineas con el ciclo
while read linea; do
    #salta las lineas que se encuentran vacias
    if [ -z "$linea" ]; then
        continue
    fi
    
    #llamamos a la funcion anterior para validar la linea
    if validar_linea "$linea"; then
        echo "$linea"
    fi
done
