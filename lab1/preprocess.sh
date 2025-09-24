#!/bin/bash


#entradas: datos anteriores de generator
#salidas: datos validados
#descripcion: ve que los datos de entrada sean validos, en temas de formato y tipo de datos y los convierte a ISO 8601

#variable de bandera
iso8601=false

#vemos los argumentos
while [ $# -gt 0 ]; do
    case $1 in #si $1 que es el argumento actual
    #es --iso8601 activa la bandera de true
        --iso8601) iso8601=true; shift ;; #ya que se valido eliminamos este argumento. para que pase al siguiente
        *) echo "error como entrega los datos"; exit 1 ;; #si tiene un argumento que no reconoce termina mostrando el mensaje de error
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
