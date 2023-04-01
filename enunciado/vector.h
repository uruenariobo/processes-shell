#define INITIAL_VECTOR_CAPACITY 8

typedef struct Vector
{
char** data; // datos almacenados en el vector
int capacity; // capacidad actual del vector
int size; // tamaño actual del vector
} Vector;


/* Devuelve una nueva instancia de Vector */
Vector create_vector();

/* Agrega la cadena dada al final del vector */
void push_back(Vector*, char*);

/* Elimina el último elemento del vector */
void pop_back(Vector*);

/* Obtiene la cadena que reside en el índice dado */
char* get(Vector*, int);

/* Devuelve la primera aparición de la clave dada en un vector. Si no se encuentra, devuelve -1 */
int search_key (Vector*, char*);

/* Libera el espacio asignado por el vector para almacenar sus elementos */
void destroy (Vector*);
