#ifndef HISTORIAL_H
#define HISTORIAL_H

#define MAX_HISTORIAL 100

struct historial {
    int longitud;
    char *comandos[MAX_HISTORIAL];
};

void agregar_comando_al_historial(char *comando, struct historial *historial_comandos);
char *obtener_comando_desde_historial(int indice, struct historial *historial_comandos);

#endif
