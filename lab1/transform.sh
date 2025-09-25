#!/bin/bash

# Entradas: datos por stdin, opción --anon-uid
# Salidas: datos transformados (UID anonimizados si se solicita)
# Descripción: anonimiza el UID con un hash dejando el resto de campos intactos

# Variable que controla si se debe anonimizar UID
anon_uid=false

# Procesar argumentos
while [ $# -gt 0 ]; do
    case $1 in
    # En caso de que se el UID no sea anonimo, se activa la flag
        --anon-uid)
            anon_uid=true
            shift
            ;;
        *)
    # Caso contrario
            echo "Uso: $0 [--anon-uid]"
            exit 1
            ;;
    esac
done

# Función para generar UID
# Entradas: UID original
# Salidas: UID anonimizdo
# Descripción: Anonimiza el UID usando md5sum
anonimizar_uid() {
    local uid="$1"
    echo -n "$uid" | md5sum | cut -c1-6
}

# Procesar cada línea de entrada
while read linea; do
    # Saltar líneas vacías
    if [ -z "$linea" ]; then
        continue
    fi

    # Separar columnas tabuladas
    timestamp=$(echo "$linea" | cut -f1)
    pid=$(echo "$linea" | cut -f2)
    uid=$(echo "$linea" | cut -f3)
    comm=$(echo "$linea" | cut -f4)
    pcpu=$(echo "$linea" | cut -f5)
    pmem=$(echo "$linea" | cut -f6)

    # Se anonimiza el UID llamando a la funcion anonimizar_uid
    if [ "$anon_uid" = true ]; then
        uid=$(anonimizar_uid "$uid")
    fi

    # Imprimir la linea transformada, tabulada
    echo -e "$timestamp\t$pid\t$uid\t$comm\t$pcpu\t$pmem"
done

