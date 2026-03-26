#include <stdio.h>
#include <stdlib.h>
#include "radio.h"
#include "queue.h"

/* Prototipo de la funcion del menu nuevo (que hay que hacer) */
int now_playing_menu(Queue *q);

int main(int argc, char *argv[]) {
    Radio *r = NULL;
    Queue *q = NULL;

    /* 1. Comprobar los argumentos de entrada */
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <fichero_radio>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Aquilogica para:
       - Leer radio desde fichero .txt
       - Insertar canciones en la cola de reproduccion
       - Llamar a now_playing_menu(q)
       - Liberar memoria
    */

    return EXIT_SUCCESS;
}

int now_playing_menu(Queue *q) {
    /* Aqui logica del menu (Next song / Exit) */
    return 0;
}