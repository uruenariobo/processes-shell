/* Este programa es una implementación básica de una consola de comandos. Permite ejecutar comandos en un sistema Unix/Linux a través de una terminal.
La consola puede tomar comandos de entrada ya sea de un archivo o de la entrada estándar. El usuario también puede redirigir la salida del comando a un archivo o ejecutar comandos en segundo plano.
El programa define un vector PATH que contiene una ruta predeterminada /bin para buscar los comandos ingresados por el usuario. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "vector.h"
#include <termios.h>
#include <ctype.h>
//#include "historial.h"

//creamos una instancia global del historial
//struct historial historial_comandos = {0, {NULL}};


/* Define el vector de rutas predeterminadas para buscar los comandos ingresados por el usuario */
Vector PATH;

//int history_index = 0; // índice del comando actual en el historial
//int history_size = 0; // número total de comandos en el historial

/* Función que toma una línea de texto y devuelve un vector de tokens lógicos */
Vector parse (char* line);

/* Esta función verifica si ambos descriptores apuntan al mismo archivo */
/* Devuelve verdadero si ambos descriptores de archivos apuntan al mismo archivo */
int isSameFile (int, int);

/* Verifica si el caracter dado es un delimitador */
/* Verifica si el carácter dado debe usarse como delimitador para parse() */
int isDelimiter (char);

/* Devuelve verdadero si el comando dado es válido en términos de sintaxis de redirección */
int isValidRedirection (Vector tokens);

/* Devuelve verdadero si el comando dado es válido en términos de la sintaxis de Ampersand */
int isValidAmpersand (Vector tokens);

/* Función que concatena dos cadenas de texto */
/* Dado dos cadenas, devuelve la concatenación de ellas */
char* concat (char* a, char* b);

/* Muestra el único mensaje de error en pantalla */
void showError ();

/**
 * Dado un comando lo utiliza como un vector de tokens, ejecuta el comando
 * y luego devuelve el ID del proceso creado
 */


int execute_command (Vector tokens);

int main (int argc, char* argv[])
{

    FILE* input_file = NULL;
    /* Verifica si se ingresó algún archivo como argumento de entrada */
    for (int i = 1; i < argc; i++) {
        FILE* cur_file = fopen(argv[i], "r");
        /* Si no se pudo abrir el archivo, muestra un mensaje de error y se sale */
        if (cur_file == NULL) {
            showError();
            exit(1);
        }
	/* Si es el primer archivo, asignarlo al puntero de entrada del archivo input_file */
        if (input_file == NULL) {
            input_file = cur_file;
        } else {
	    /* Si no es el primer archivo, verificar si es el mismo archivo que el anterior */
            if (!isSameFile(fileno(input_file), fileno(cur_file))) {
                /* Si no es el mismo archivo, mostrar mensaje de error y salir */
                showError();
                exit(1);
            }
        }
    }

    /*Si el archivo de entrada ya ha sido definido, se reasigna la entrada estándar de C al archivo*/
    if (input_file != NULL) stdin = fdopen(fileno(input_file), "r");

	/*Se agrega la ruta "/bin" al final del vector PATH*/
    push_back(&PATH, "/bin");

    

    while (1) // Bucle infinito para la lectura de comandos
    {
        if (input_file == NULL) printf("wish> "); /*Si no se está leyendo un archivo de entrada, se muestra el indicador de línea de comandos */
        char* line = NULL; // Se inicializa un puntero a char llamado "line" en NULL
        size_t n = 0;  // Se inicializa una variable de tipo size_t llamada "n" en 0
        int len = getline(&line, &n, stdin); /* Se lee una línea de entrada del usuario y se almacena en "line", y se guarda el número de caracteres leídos en "len" */ 
        if (len == -1) { // Si el valor devuelto por getline es -1, significa que se ha producido un error al leer la línea de entrada
            if (input_file == NULL) continue; // Si no se está leyendo un archivo de entrada, se continúa con la siguiente iteración del bucle. Si se está leyendo un archivo de entrada, se sale del bucle
            break;
        }
        line[len-1] = '\0'; // Se reemplaza el carácter de nueva línea al final de la línea leída por un carácter nulo
        Vector tokens = parse(line); // Se llama a la función "parse" con la línea leída como argumento, lo que devuelve un Vector con los tokens obtenidos de la línea
        if (!isValidAmpersand(tokens)) {  // Si los tokens no contienen un uso válido del operador "&" se continua con la siguiente iteraciòn del bucle
            continue;
        }
        int commandsCount = 1; // Se inicializa una variable entera llamada "commandsCount" en 1
        for (int i = 0; i < tokens.size; i++) { // Se recorren todos los tokens
            if (strcmp("&", get(&tokens, i)) == 0) commandsCount++; // Si el token actual es "&", se incrementa "commandsCount"
        }
        Vector command = create_vector(); // Se crea un nuevo Vector vacío llamado "command"
        int processIds[commandsCount]; // Se declara un array de enteros llamado "processIds" con el tamaño "commandsCount"
        int sz = 0;
        for (int i = 0; i < tokens.size; i++) { // Se recorren todos los tokens para construir os comandos y ejecutarlos
            if (strcmp("&", get(&tokens, i)) != 0) { // Si el token actual no es "&" Se añade el token al final del Vector "command"
                push_back(&command, get(&tokens, i));
            } else {
                continue; // Si el token actual es "&", se continúa con la siguiente iteración del bucle
            }

            // Se verifivca si el token actual es el último de la línea o es "&"
            // si se encuentra un ampersand al final de la linea o en medio de los comandos, se ejecuta el comando y se almacena el PID
            if (
                i == tokens.size - 1 ||
                strcmp("&", get(&tokens, i+1)) == 0
            ) {
                // Se verifica si el Vector "command" no contiene una redirección válida, en cuyo caso se sale y cierra el bucle
                if (!isValidRedirection(command)) { 
                    showError();
                    break;
                }

                // Se ejecuta el comando y se almacena su PID si es valido
                int res = execute_command(command);
                if (res != -1) processIds[sz++] = res;
                command = create_vector();
            }
        }

        // se espera a que terminen de ejecutarse los comandos en paralelo
        for (int i = 0; i < sz; i++) waitpid(processIds[i], NULL, 0);

        // Se libera la memoria y se destruyen los comandos en paralelo
        free(line);
        destroy(&tokens);
        destroy(&command);
    }

    // HISTORIAL* con esto se restaura la forma anterior de leer comandos
    //tcsetattr(STDIN_FILENO, TCSANOW, &old_term);

return 0;
}

//esta función recibe un carácter y devuelve 1 si es un delimitador válido 
//o 0 en caso contrario. Los delimitadores válidos son el espacio, 
//la tabulación, la redirección > y el operador &.
int isDelimiter (char c) {
    return (c == ' ') || (c == '\t') || (c == '>') || (c == '&');
}


//esta función recibe una línea de entrada del usuario 
//y devuelve un vector de tokens que representan los comandos 
//y argumentos a ejecutar. Para esto, recorre la línea de 
//entrada carácter por carácter y separa los tokens según 
//los delimitadores válidos encontrados. 
//Los tokens son almacenados en un vector.
Vector parse (char* line) {
    Vector ans = create_vector();
    int n = strlen(line);
    char* s = NULL;
    int start = -1;
    for (int i = 0; i < n; i++) {
        if (line[i] == '>') {
            push_back(&ans, ">");
            continue;
        }
        if (line[i] == '&') {
            push_back(&ans, "&");
            continue;
        }
        if (!isDelimiter(line[i])) {
            if (i == 0 || isDelimiter(line[i-1])) start = i;
            if (i == n-1 || isDelimiter(line[i+1])) {
                s = strndup(line+start, i-start+1);
                push_back(&ans, s);
            }
        }
    }
    return ans;
}

//esta función recibe dos descriptores de archivo 
//y devuelve 1 si ambos archivos son el mismo 
//y 0 en caso contrario. Esto se determina comparando 
//los identificadores de dispositivo y de ***inodo del sistema
// de archivos.
int isSameFile (int fd1, int fd2) {
    struct stat stat1, stat2;
    if(fstat(fd1, &stat1) < 0) return -1;
    if(fstat(fd2, &stat2) < 0) return -1;
    return (stat1.st_dev == stat2.st_dev) && (stat1.st_ino == stat2.st_ino);
}

//esta función recibe un vector de tokens 
//y devuelve 1 si la redirección especificada
// es válida y 0 en caso contrario. Una redirección 
//es válida si se encuentra en la posición correcta 
//(antes del último token) y no hay redirecciones consecutivas.
// La función utiliza la función strcmp() para comparar los tokens
// con el carácter >.
int isValidRedirection (Vector tokens) {
    int n = tokens.size;
    for (int i = 0; i < n; i++) {
        if (strcmp(">", get(&tokens, i)) == 0) {
            if (i == 0 || i == n-1 || n-1-i > 1) return 0;
            if (i != n-1 && strcmp(">", get(&tokens, i+1)) == 0) return 0;
        }
    }
    return 1;
}

//esta función recibe un vector de tokens 
//y devuelve 1 si el operador ampersand especificado 
//es válido y 0 en caso contrario. Un operador ampersand 
//es válido si no se encuentra en la primera posición 
//y no hay operadores consecutivos. La función utiliza 
//la función strcmp() para comparar los tokens con el carácter &
int isValidAmpersand (Vector tokens) {
    int n = tokens.size;
    for (int i = 0; i < n; i++) {
        if (strcmp("&", get(&tokens, i)) == 0) {
            if (i == 0) return 0;
            if (i != n-1 && strcmp("&", get(&tokens, i+1)) == 0) return 0;
        }
    }
    return 1;
}

//esta función concatena dos cadenas de caracteres a y b 
//y devuelve el resultado. Primero se reserva memoria suficiente
// para la nueva cadena, luego se copia a 
//y finalmente se concatena b.
char* concat (char* a, char* b) {
    char* ans = (char*) malloc((strlen(a) + strlen(b) + 1) * sizeof(char));
    strcpy(ans, a);
    strcat(ans, b);
    return ans;
}

//esta función imprime un mensaje de error 
//predefinido en la salida de error estándar.
void showError () {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message)); 
}

//esta función recibe un vector de tokens que representa un comando
// y sus argumentos, y ejecuta el comando correspondiente. 
//Primero se comprueba si el comando es "exit", "cd" o "path",
// y se ejecuta la acción correspondiente. 
//Si no, se busca el archivo ejecutable en las rutas especificadas
// en la variable PATH. Si se encuentra el archivo, 
//se crea un proceso hijo y se ejecuta el archivo. 
//Si hay una redirección a archivo especificada, 
//se redirige la salida estándar del proceso hijo al archivo 
//especificado. La función devuelve el identificador del proceso hijo
// o -1 si hubo un error.
int execute_command (Vector tokens) {

    char* command = get(&tokens, 0);
    if (strcmp(command, "exit") == 0) {
        if (tokens.size != 1) showError();
        else exit(0);
        return -1;
    }
    if (
        strcmp(command, "cd") == 0
    ) {
        if (tokens.size != 2) showError();
        else chdir(get(&tokens, 1));
        return -1;
    }
    if (tokens.size >= 1 && strcmp("path", command) == 0) {
        Vector params = create_vector();
        for (int i = 1; i < tokens.size; i++) push_back(&params, get(&tokens, i));
        PATH = params;
        return -1;
    }

    // Agregar comando al historial
    //char* linea = vector_to_string(tokens);
    //agregar_comando_al_historial(linea, &historial_comandos);
    
    /**
     * Spawn the required process to execute the command
     * Supports redirection to files
     */
    int pos = search_key(&tokens, ">");
    if (pos == -1) pos = tokens.size;
    for (int i = 0; i < PATH.size; i++) {
        char* p = concat(get(&PATH, i), concat("/", command));
        if (access(p, X_OK) == 0) {
            char* argv[pos+1];
            for (int i = 0; i < pos; i++) argv[i] = get(&tokens, i);
            argv[pos] = NULL;
            
            int rc = fork();
            if (rc == 0) {
                if (pos != tokens.size) {
                    close(STDOUT_FILENO);
                    open(
                        get(&tokens, tokens.size - 1),
                        O_CREAT | O_WRONLY | O_TRUNC,
                        S_IRWXU
                    );
                }
                execv(p, argv);
            }
            return rc;
        }
    }

    showError();
    return -1;
}
