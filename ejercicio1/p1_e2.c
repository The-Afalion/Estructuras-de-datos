#include <stdio.h>
#include "radio.h"

int main() {
    Radio *r = NULL;

    printf("Initializing radio...\n");
    r = radio_init();
    if (!r) {
        printf("Error initializing radio.\n");
        return 1;
    }

    printf("\n--- Adding songs ---\n");
    radio_newMusic(r, "id:\"10\" title:\"Song A\" artist:\"Artist 1\" duration:\"180\"");
    radio_newMusic(r, "id:\"20\" title:\"Song B\" artist:\"Artist 2\" duration:\"220\"");
    radio_newMusic(r, "id:\"30\" title:\"Song C\" artist:\"Artist 3\" duration:\"200\"");

    printf("Total songs in radio: %d\n", radio_getNumberOfMusic(r));

    printf("\n--- Adding relations ---\n");
    printf("Adding relation 10 -> 20\n");
    radio_newRelation(r, 10, 20);
    printf("Adding relation 10 -> 30\n");
    radio_newRelation(r, 10, 30);
    printf("Adding relation 30 -> 10 (cycle)\n");
    radio_newRelation(r, 30, 10);

    printf("Total relations in radio: %d\n", radio_getNumberOfRelations(r));

    printf("\n--- Checking relations ---\n");
    printf("Relation 10 -> 20 exists? (1=YES, 0=NO): %d\n", radio_relationExists(r, 10, 20));
    printf("Relation 20 -> 10 exists? (1=YES, 0=NO): %d\n", radio_relationExists(r, 20, 10));
    printf("Relation 30 -> 10 exists? (1=YES, 0=NO): %d\n", radio_relationExists(r, 30, 10));

    printf("\n--- Printing full radio ---\n");
    radio_print(stdout, r);

    printf("\nFreeing radio memory...\n");
    radio_free(r);

    printf("Done.\n");
    return 0;
}
