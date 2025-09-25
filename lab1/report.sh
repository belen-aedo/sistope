#!/bin/bash

# Entradas: datos por stdin, -o archivo de salida
# Salidas: archivo TSV con reporte final incluyendo metadatos
# Descripción: agrega metadatos (fecha, usuario, host) y escribe toda la entrada
#              en un archivo de salida especificado por el usuario.

# Variable para el archivo de salida
output_file=""

# Leer argumentos
while [ $# -gt 0 ]; do
    case $1 in
        -o) output_file=$2; shift 2 ;;  # Guardar el nombre del archivo de salida
        *) echo "Uso: $0 -o archivo_salida"; exit 1 ;;
    esac
done

# Validar que se haya dado un archivo de salida
if [ -z "$output_file" ]; then
    echo "Debes especificar un nombre para el archivo de salida con -o NOMBRE_ARCHIVO"
    exit 1
fi

# Procesar la entrada y generar el reporte
# Entradas: datos por stdin
# Salidas: archivo con metadatos + datos
# Descripción: escribe la fecha, usuario y host como comentarios luego agrega la entrada completa en el archivo de salida
{
    echo "# Reporte generado: $(date '+%Y-%m-%dT%H:%M:%S%z')"
    echo "# Usuario: $USER"
    echo "# Host: ${HOSTNAME:-$(hostname)}"
    while read linea; do
        echo "$linea"
    done
} > "$output_file"

# Mensaje de confirmacion de creacion de archivo
echo "Reporte generado en: $output_file"
                                 