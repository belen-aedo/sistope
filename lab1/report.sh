#!/bin/bash


# Entradas: datos por stdin, -o archivo de salida
# Salidas: archivo TSV con reporte final incluyendo metadatos
# Descripcion: agrega metadatos (fecha, usuario, host) y escribe toda la entrada
#              en un archivo de salida especificado por el usuario.

#variable para el nombre de archivo de salida
output_file=""

#leemos los argumenots
while getopts "o:" opcion; do
    case $opcion in
        o)
            output_file=$OPTARG
            ;;
        *)
            echo "Uso: $0 -o <archivo_salida.tsv>" >&2
            exit 1
            ;;
    esac
done

# Validar que se haya dado un archivo de salida
if [ -z "$output_file" ]; then
    echo "Debes especificar un nombre para el archivo de salida con -o NOMBRE_ARCHIVO"
    exit 1
fi

#obtenemos los metadatos
usuario=$(whoami)
host=$(hostname)


#Entradas: datos por stdin
#Salidas: archivo con metadatos + datos
#Descripcion: escribe la fecha, usuario y host como comentarios luego agrega toda la entrada
{
    echo "# Reporte generado: $(date +"%Y-%m-%dT%H:%M:%S+00:00")"
    echo "# user: $usuario"
    echo "# host: $host"
    
    #agregar todos los datos de stdin
    cat
    
} > "$output_file"

echo "Reporte guardado en: $output_file"
