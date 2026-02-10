#include <stdio.h>
#include "radio.h"

int main(int argc, char *argv[]) {
    Radio *r = NULL;
    FILE *f = NULL;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        return 1;
    }

    f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "Error opening file %s\n", argv[1]);
        return 1;
    }

    printf("Initializing radio...\n");
    r = radio_init();
    if (!r) {
        printf("Error initializing radio.\n");
        fclose(f);
        return 1;
    }

    printf("Reading radio from file %s...\n", argv[1]);
    if (radio_readFromFile(f, r) == ERROR) {
        fprintf(stderr, "Error reading radio data from file.\n");
        radio_free(r);
        fclose(f);
        return 1;
    }

    printf("\n--- Radio loaded successfully ---\n");
    printf("Total songs: %d\n", radio_getNumberOfMusic(r));
    printf("Total relations: %d\n", radio_getNumberOfRelations(r));

    printf("\n--- Printing full radio ---\n");
    radio_print(stdout, r);

    printf("\nFreeing radio memory...\n");
    radio_free(r);
    fclose(f);

    printf("Done.\n");
    return 0;
}
