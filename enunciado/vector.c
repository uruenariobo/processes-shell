#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "vector.h"


// Crea un nuevo vector inicializado con capacidad y tamaño 0
Vector create_vector () {
    Vector v;
    v.data = (char**) malloc(INITIAL_VECTOR_CAPACITY * sizeof(char*));
    v.capacity = INITIAL_VECTOR_CAPACITY;
    v.size = 0;
    return v;
}

// Agrega un nuevo elemento al final del vector, expandiendo la capacidad del vector si es necesario
void push_back(Vector* v, char* val) {
    if (v->capacity == v->size) {
        char** new_data = (char**) malloc( (v->capacity << 1) * sizeof(char*));// Duplica la capacidad actual del vector
        int n = v->size;
        for (int i = 0; i < n; i++) new_data[i] = v->data[i];// Copia los datos del vector anterior al nuevo vector
        free(v->data); // Libera la memoria del vector anterior
        v->data = new_data; // Asigna el nuevo vector como el vector de datos
        v->capacity <<= 1;// Actualiza la capacidad del vector al doble
    }
    v->data[v->size++] = val; // Agrega el nuevo elemento al final del vector
}

// Elimina el último elemento del vector
void pop_back(Vector* v) {
    if (v->size > 0) v->size--; // Si el vector tiene al menos un elemento, lo elimina reduciendo el tamaño
}

// Devuelve el elemento del vector en la posición dada
char* get (Vector* v, int index) {
    if (index < 0 || index >= v->size) return NULL; // Si el índice está fuera del rango válido, devuelve NULL
    return v->data[index]; // Devuelve el elemento del vector en la posición dada
}

// Devuelve el tamaño actual del vector
int get_size(Vector* v) {
    return v->size;
}

// Busca un elemento en el vector y devuelve su índice, o -1 si no lo encuentra
int search_key (Vector* v, char* key) {
    int n = v->size;
    for (int i = 0; i < n; i++) {
        if (strcmp(key, v->data[i]) == 0) return i; // Compara el elemento actual del vector con la clave dada
    }
    return -1; // Si no se encuentra la clave, devuelve -1
}

// Libera la memoria del vector
void destroy (Vector* v) {
    free(v->data);
}
