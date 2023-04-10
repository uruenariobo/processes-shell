#! /bin/bash

# Verifica si el archivo ejecutable "wish" existe y tiene permisos de ejecución
if ! [[ -x wish ]]; then
    # Si el archivo no existe o no tiene permisos de ejecución, muestra un mensaje de error
    echo "wish executable does not exist"
    exit 1 # Y sale del script con un código de error 1
fi

# Ejecuta el script "run-tests.sh" ubicado en la carpeta "tester", pasando los argumentos recibidos
# en la línea de comandos (* significa que se pasan todos los argumentos)

../tester/run-tests.sh $*
