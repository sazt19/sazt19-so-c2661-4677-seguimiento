#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Uso: $0 <directorio_trabajo>"
    exit 1
fi

dir="$1"

if [ ! -d "$dir" ]; then
    echo "Error: no es un directorio válido"
    exit 1
fi

find "$dir" -type f -name "*.keep" -exec git rm {} \;
