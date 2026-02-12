#include <stdio.h>
#include "music.h"

int main() {
    Music   *m1 = NULL, *m2 = NULL, *m3 = NULL;
    Bool    noEqual;

    printf("Initializing songs...\n");
    m1 = music_init();
    m2 = music_init();


    if (!m1 || !m2) {
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
    
    noEqual = music_cmp((void *)m1, (void *)m2);

    // Configure m3 (copy of m1 to test comparison)
    m3 = (Music *)music_copy(m1);
    if(m3 == NULL) 
    {
        printf("Error initializing songs.\n");
        return 1;
    }

    //Print of the firsts songs
    // printf("\n--- Printing Song 1 ---\n");
    music_formatted_print(stdout, m1);

    // printf("\n--- Printing Song 2 ---\n");
    music_formatted_print(stdout, m2);

    //Comparing funtions

    // printf("\n--- Comparing Songs ---\n");
    printf("Equals? ");
     
    
    
    //print title funtion
    printf("Music 2 title: %s\n", music_getTitle(m2));

    //print title funtion
    printf("Music 3 id: %ld\n", music_getId(m3));


    //Print song 1 and song 3
    // printf("\n--- Printing Song 1 ---\n");
    music_formatted_print(stdout, m1);
    // printf("\n--- Printing Song 3 ---\n");
    music_formatted_print(stdout, m3);

    //comparing son 3 whit 
    noEqual = music_cmp((void *)m1, (void *)m3);
    // printf("\n--- Comparing Songs ---\n");
    
    printf("Equals? ");
    if(noEqual == FALSE) 
        printf("Yes\n"); 
    else 
        printf("NO\n");
    
    





    // printf("Comparing Song 1 and Song 2 (should be < 0): %d\n", music_cmp(m1, m2));
    // printf("Comparing Song 2 and Song 1 (should be > 0): %d\n", music_cmp(m2, m1));
    // printf("Comparing Song 1 and Song 3 (should be 0): %d\n", music_cmp(m1, m3));

    // printf("\nFreeing memory...\n");
    music_free(m1);
    music_free(m2);
    music_free(m3);

    return 0;
}
