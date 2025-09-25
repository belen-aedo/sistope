#!/bin/bash

# Entradas: datos por stdin
# Salidas: métricas agregadas
# Descripción: calcula promedios y conteos de CPU/MEM de los procesos

#variables
total_cpu=0
total_mem=0
count=0


while read linea; do
    # Si la linea esta vacia, la salta
    if [ -z "$linea" ]; then
        continue
    fi

    # Extraer los valores de CPU y Memoria usando cut (columnas 5 y 6)
    pcpu=$(echo "$linea" | cut -f5)
    pmem=$(echo "$linea" | cut -f6)

    # Acumular los totales de CPU y Memoria (se ocupa awk para sumar decimales)
    total_cpu=$(awk "BEGIN{print $total_cpu+$pcpu}")
    total_mem=$(awk "BEGIN{print $total_mem+$pmem}")
    count=$((count + 1))
done

if [ $count -gt 0 ]; then
    # Calcular promedios
    prom_cpu=$(awk "BEGIN{printf \"%.2f\", $total_cpu/$count}")
    prom_mem=$(awk "BEGIN{printf \"%.2f\", $total_mem/$count}")

    echo -e "Procesos:\t$count"
    echo -e "CPU promedio:\t$prom_cpu"
    echo -e "MEM promedio:\t$prom_mem"
else
    echo "No se recibieron datos."
fi
