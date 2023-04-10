# Shell WISH UdeA


Lo primero que hicimos para ejecutar los test es verificar que run-test.sh tuviera los permisos para ejecutarse desde consola así buscamos ../tester/run-test.sh es la correcta y que el archivo run-test.sh existe en la carpeta tester dentro de la carpeta anterior (..) a la que te encuentras actualmente. Con esto verificamos que tenemos los permisos necesarios para ejecutar el archivo run-test.sh. Se puede hacer ejecutando el siguiente comando en la terminal:


ls -l ../tester/run-test.sh

Este comando muestra los permisos del archivo inicialmente. Como no teníamos los permisos de ejecución, le dimos permiso ejecutando siguiente comando:

chmod +x ../tester/run-test.sh


#Act1.

Su shell basico será llamadao wish (abreviación para Wisconsin Shell, de la cual se tomó esta practica), un shell es basicamente un loop interactivo que: imprime repetidamente el promp wish>  (nota: despues del signo > hay un espacio), analiza (parse) el comando ingresado a la entrada, ejecuta dicho comando y espera a que este finalice. Este proceso es repetido hasta que el usuario digite exit. Una vez compile el código, el nombre final de su ejecutable será wish.

Creo entonces que es crear el programa que permita ejecutar el loop.

#Act 2. 

El shell puede ser invocado tanto sin argumentos o con unico argumento; cualquier otra cosa generará un error. A continuación se muestra como sería la ejecución para el caso sin argumento:

prompt> ./wish
wish>

Creo que es crear la forma de invocarlo y generar el modo de excepción que arroje error.

#Act3

El shell tambien soporta un batch mode, el cual lee la entrada desde un batch file y ejecuta los comandos contenidos en este. A continuación se muestra como el shell wish ejecutaria un batch file llamado batch.txt:

prompt> ./wish batch.txt

Creo que es crear el modo batch

#Act4

Usted deberia estructurar su shell de manera que cree un procesos para cada nuevo comando (con los built-in commands, que se discutirán abajo, como excepción). El shell basico deberá ser capaz de analizar (parser) un comando y correr el programa correspondiente a ese comando. Por ejemplo, si el usuario digita ls -la /tmp, el shell deberia ejecutar el programa /bin/ls con los argumentos dados -la y /tmp (¿Como sabe el shell ejecutar /bin/ls?. Es algo llamado shell path; mas de esto abajo)

Creo que es...

Act4. Crear la invocación ls

Con los argumentos la /temp

# Act 5 

El shell es muy simple (conpeptualmente): Este corre en un ciclo infinito solicitando repetidamente una entrada que dice el comando a ejecutar. Luego ejecuta ese comando. El ciclo continua indefinidamente hasta que el usuario escribe el comando integrado (built-in ) exit, el cual hace que se salga del shell.

Creo o supongo 😅 que es crear la.invocacion exit

#Act6

Sin embargo, el shell tambien soportara el batch mode, en el cual al shell se le da como entrada un archivo de comandos; en este caso, el shell no leerá la entrada de usuario (de stdin) sino que la entrada será tomada desde un archivo que contendrá los comandos a ejecutar.

Asumo que es generar los comandos a leer desde un archivo y no desde un terminal 🤔🤔

Esto confirma la hipótesis de la act6 o lo que creo que hay que hacer:

En cada modo, si se encuentra un marcador de fin de archivo EOF (end-of-file marker), se deberá invocar la llamada exit(0) para salir.

Luego en Path, entiendo que se explican las formas correctas de acceder al path, En comandos built in, se encuentra una tarea

#Act7

En este proyecto, usted deberá implementar exit, cd, y path como comandos integrados (build.in commnads).

En Redirection entiendo que está como debemos mostrar la salida, podríamos decir que es la siguiente actividad

#Act8
...Por ejemplo, si un usuario escribe ls -la /tmp > output, no se debe imprimir nada en la pantalla. En su lugar, la salida estándar del programa ls debería redireccionarse archivo output. Además, la salida de error estándar del programa debe redirigirse al archivo output (el giro es que es un poco diferente a la redirección estándar).

Si el archivo output existe antes de ejecutar su programa, simplemente debe sobrescribirlo (después de truncarlo)...

El formato exacto de la redirección es un comando (y posiblemente algunos argumentos) seguido del símbolo de redirección seguido de un nombre de archivo. Varios operadores de redirección o varios archivos a la derecha del signo de redirección son errores.

#Act9

En la consola de Linux, cuando usted oprime la tecla Flecha arriba (UP arrow), usted puede navegar en el historial de los últimos comandos que se han utilizado. Usted debe implementar esta funcionalidad garantizando un historial de hasta 20 comandos.

Entonces, cómo según lo que leemos la lectura y la salida no son por consola, creeooo🤔 que deberíamos hacer una salida del historial de los 20 últimos elementos, por mi le ponemos el historial de todo 😅

#Act10

El shell también deberá permitir al usuario ejecutar comandos paralelos. Esto se logra con el operador ampersand (&) de la siguiente manera:

wish> cmd1 & cmd2 args1 args2 & cmd3 args1
En este caso, en lugar de ejecutar cmd1 y luego esperar a que termine, su shell debe ejecutar cmd1, cmd2 y cmd3 (cada uno con los argumentos que el usuario le haya pasado) en paralelo, antes de esperar a que se complete alguno de ellos.

Asumo, pienso, creo, entiendo que ... Hay que permitir invocar paralelamente al la Shell cada que se escriba un nuevo &

Incluyendo esto...

Luego, después de iniciar todos estos procesos, asegurese de usar wait() (o waitpid) para esperar a que se completen. Una vez finalizados todos los procesos, devuelva el control al usuario como de costumbre (o, si está en batch mode, pase a la siguiente línea).

#Act11

Solo hay un mensaje de error el cual se debe imprimir cada vez que se encuentre un error de cualquier tipo:

char error_message[30] = "An error has occurred\n";
write(STDERR_FILENO, error_message, strlen(error_message); 
El mensaje de error debe ser impreso a stderr (standard error), como se mostró arriba.

Sinónimo de creo que es implementar la salida de error

Y al final en MISCELANUS HINTS encontramos los consejos para hacer el desarrollo

Test 1: Entrada para comprobar cd defectuoso. No se pasan argumentos a cd.
Test 2:  los argumentos se pasan a cd.
Test 3: ls con un nombre de directorio incorrecto.
Test 4: Entrada para ejecutar misc. comandos
Test 5: Intenta salir con una discusión. Debería arrojar un error.
Test 6: Intente ejecutar un script de shell sin configurar la ruta.
Test 7: Establezca la ruta, ejecute un script de shell. Sobrescriba la ruta y luego intente ejecutar el script nuevamente.
Test 8: Redirección sin archivo de salida especificado.
Test 9: Redirección con múltiples archivos de salida.
Test 10: Redirigir con múltiples '>'
Test 11: Redirección normal.
12- Entrada para verificar la redirección incorrecta. No contiene ningún comando antes de '>'.
13- El archivo de entrada es válido y vacío. Pero la shell se invoca en modo por lotes con 2 archivos (con el mismo archivo 13.in usado dos veces).
14- La shell se invoca con un archivo de lotes incorrecto.
15- Prueba de comando con espacios variables.
16- El comando solo contiene '&'.
17- El comando paralelo contiene '&' al final.
18- Prueba básica de ejecución de comandos en paralelo.
19- Comandos paralelos sin espacio entre el comando y '&'
20- Combinación de comandos de redirección y en paralelo.
21- Comandos vacíos.
22- Prueba para verificar que los comandos no se ejecutan en serie.



