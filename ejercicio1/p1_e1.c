#include <stdio.h>
#include "music.h"

int main() {
    Music *m1 = NULL, *m2 = NULL, *m3 = NULL;

    printf("Initializing songs...\n");
    m1 = music_init();
    m2 = music_init();
    m3 = music_init();

    if (!m1 || !m2 || !m3) {
        printf("Error initializing songs.\n");
        return 1;
    }

    // Configure m1
    music_setId(m1, 10);
    music_setTitle(m1, "Blinding Lights");
    music_setArtist(m1, "The Weeknd");
    music_setDuration(m1, 200);
    music_setState(m1, LISTENED);

    // Configure m2
    music_setId(m2, 20);
    music_setTitle(m2, "Bohemian Rhapsody");
    music_setArtist(m2, "Queen");
    music_setDuration(m2, 355);
    music_setState(m2, NOT_LISTENED);

    // Configure m3 (copy of m1 to test comparison)
    music_setId(m3, 10);
    music_setTitle(m3, "Blinding Lights");
    music_setArtist(m3, "The Weeknd");
    music_setDuration(m3, 200);
    music_setState(m3, LISTENED);

    printf("\n--- Printing Song 1 ---\n");
    music_formatted_print(stdout, m1);

    printf("\n--- Printing Song 2 ---\n");
    music_formatted_print(stdout, m2);

    printf("\n--- Comparing Songs ---\n");
    printf("Comparing Song 1 and Song 2 (should be < 0): %d\n", music_cmp(m1, m2));
    printf("Comparing Song 2 and Song 1 (should be > 0): %d\n", music_cmp(m2, m1));
    printf("Comparing Song 1 and Song 3 (should be 0): %d\n", music_cmp(m1, m3));

    printf("\nFreeing memory...\n");
    music_free(m1);
    music_free(m2);
    music_free(m3);

    printf("Done.\n");
    return 0;
}
