#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "historial.h"
#include "vector.h"

#define MAX_INPUT 512
#define MAX_HISTORIAL 10

char* vector_to_string(Vector v) {
    char *result = malloc(MAX_INPUT * sizeof(char));
    result[0] = '\0';
    for (int i = 0; i < v.size; i++) {
        strcat(result, get(&v, i));
        strcat(result, " ");
    }
    return result;
}

void agregar_comando_al_historial(char *comando, struct historial *historial_comandos) {
    if (strlen(comando) > 0) {
        if (historial_comandos->longitud < MAX_HISTORIAL) {
            historial_comandos->comandos[historial_comandos->longitud] = strdup(comando);
            historial_comandos->longitud++;
        } else {
            free(historial_comandos->comandos[0]);
            for (int i = 0; i < historial_comandos->longitud - 1; i++) {
                historial_comandos->comandos[i] = historial_comandos->comandos[i+1];
            }
            historial_comandos->comandos[historial_comandos->longitud - 1] = strdup(comando);
        }
    }
}

char *obtener_comando_desde_historial(int indice, struct historial *historial_comandos) {
    if (indice >= 0 && indice < historial_comandos->longitud) {
        return historial_comandos->comandos[indice];
    }
    return NULL;
}
