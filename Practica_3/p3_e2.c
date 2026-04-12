#include <stdio.h>
#include <stdlib.h>

#include "radio.h"

#define ERROR_MAIN 1
#define MIN_ARGC 4

int main(int argc, char **argv) {
    FILE *fin = NULL;
    Radio *radio = NULL;
    long from_id, to_id;

    if (argc < MIN_ARGC) {
        fprintf(stderr, "Usage: %s <radio.txt> <from_id> <to_id>\n", argv[0]);
        return ERROR_MAIN;
    }

    fin = fopen(argv[1], "r");
    if (!fin) {
        fprintf(stderr, "ERROR: Could not open input file\n");
        return ERROR_MAIN;
    }

    radio = radio_init();
    if (!radio) {
        fclose(fin);
        fprintf(stderr, "ERROR: Could not create the radio\n");
        return ERROR_MAIN;
    }

    if (radio_readFromFile(fin, radio) == ERROR) {
        fclose(fin);
        radio_free(radio);
        fprintf(stderr, "ERROR: Could not read the radio\n");
        return ERROR_MAIN;
    }
    fclose(fin);

    from_id = atol(argv[2]);
    to_id = atol(argv[3]);

    printf("Radio:\n");
    radio_print(stdout, radio);

    printf("\nFrom music with id: %ld\n", from_id);
    printf("To music with id: %ld\n", to_id);

    printf("\nBFS exploration path:\n");
    if (radio_breadthSearch(radio, from_id, to_id) == ERROR) {
        radio_free(radio);
        fprintf(stderr, "ERROR: BFS could not reach the destination\n");
        return ERROR_MAIN;
    }

    printf("\nDFS exploration path:\n");
    if (radio_depthSearch(radio, from_id, to_id) == ERROR) {
        radio_free(radio);
        fprintf(stderr, "ERROR: DFS could not reach the destination\n");
        return ERROR_MAIN;
    }

    radio_free(radio);
    return 0;
}
