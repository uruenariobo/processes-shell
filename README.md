# Shell WISH UdeA


Lo primero que hicimos para ejecutar los test es verificar que run-test.sh tuviera los permisos para ejecutarse desde consola así buscamos ../tester/run-test.sh es la correcta y que el archivo run-test.sh existe en la carpeta tester dentro de la carpeta anterior (..) a la que te encuentras actualmente. Con esto verificamos que tenemos los permisos necesarios para ejecutar el archivo run-test.sh. Se puede hacer ejecutando el siguiente comando en la terminal:


ls -l ../tester/run-test.sh

Este comando muestra los permisos del archivo inicialmente. Como no teníamos los permisos de ejecución, le dimos permiso ejecutando siguiente comando:

chmod +x ../tester/run-test.sh
