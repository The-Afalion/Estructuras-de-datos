#include <stdio.h>

#include "radio.h"

int main(int argc, char* argv[]) {
    Radio* r = NULL;
    FILE* f = NULL;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        return 1;
    }

    f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "Error opening file %s\n", argv[1]);
        return 1;
    }

    r = radio_init();
    if (!r) {
        printf("Error initializing radio.\n");
        fclose(f);
        return 1;
    }
    // Read the file
    if (radio_readFromFile(f, r) == ERROR) {
        fprintf(stderr, "Error reading radio data from file.\n");
        radio_free(r);
        fclose(f);
        return 1;
    }

    // SHOW
    printf("\nRadio recommendations:\n");
    radio_print(stdout, r);

    radio_free(r);
    fclose(f);

    return 0;
}
