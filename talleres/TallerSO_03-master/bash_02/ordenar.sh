#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Uso: $0 <nombre_fichero> <columna>"
    exit 1
fi

fichero="$1"
columna="$2"

awk -F'|' -v col="$columna" 'NF > 0 { print $col }' "$fichero" | sort -r
