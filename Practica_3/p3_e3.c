#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "music.h"
#include "radio.h"

#define ERROR_MAIN 1
#define MIN_ARGC 2

static int music_list_print(FILE *pf, const void *m);

int main(int argc, char **argv) {
    FILE *fin = NULL;
    Radio *radio = NULL;
    List *favorites = NULL;
    Music *song = NULL;
    int i, num_music, half;

    if (argc < MIN_ARGC) {
        fprintf(stderr, "Usage: %s <radio.txt>\n", argv[0]);
        return ERROR_MAIN;
    }

    fin = fopen(argv[1], "r");
    if (!fin) {
        fprintf(stderr, "ERROR: Could not open input file\n");
        return ERROR_MAIN;
    }

    radio = radio_init();
    favorites = list_new();
    if (!radio || !favorites) {
        fclose(fin);
        radio_free(radio);
        list_free(favorites);
        fprintf(stderr, "ERROR: Could not create radio or list\n");
        return ERROR_MAIN;
    }

    if (radio_readFromFile(fin, radio) == ERROR) {
        fclose(fin);
        radio_free(radio);
        list_free(favorites);
        fprintf(stderr, "ERROR: Could not read the radio\n");
        return ERROR_MAIN;
    }
    fclose(fin);

    num_music = radio_getNumberOfMusic(radio);
    if (num_music == ERROR_I) {
        radio_free(radio);
        list_free(favorites);
        fprintf(stderr, "ERROR: Could not get the number of songs\n");
        return ERROR_MAIN;
    }

    for (i = 0; i < num_music; i++) {
        song = radio_getMusicAt(radio, i);
        if (!song) {
            radio_free(radio);
            list_free(favorites);
            fprintf(stderr, "ERROR: Could not access one song from the radio\n");
            return ERROR_MAIN;
        }

        if (i % 2 == 0) {
            if (list_pushBack(favorites, song) == ERROR) {
                radio_free(radio);
                list_free(favorites);
                fprintf(stderr, "ERROR: Could not insert at the back of the list\n");
                return ERROR_MAIN;
            }
        } else {
            if (list_pushFront(favorites, song) == ERROR) {
                radio_free(radio);
                list_free(favorites);
                fprintf(stderr, "ERROR: Could not insert at the front of the list\n");
                return ERROR_MAIN;
            }
        }
    }

    printf("Favorites list after insertion:\n");
    list_print(stdout, favorites, music_list_print);
    printf("\n");

    half = num_music / 2;

    printf("\nExtracting first half from front:\n");
    for (i = 0; i < half; i++) {
        song = (Music *)list_popFront(favorites);
        if (!song || music_plain_print(stdout, song) < 0 || printf("\n") < 0) {
            radio_free(radio);
            list_free(favorites);
            fprintf(stderr, "ERROR: Could not extract from the front of the list\n");
            return ERROR_MAIN;
        }
    }

    printf("\nExtracting second half from back:\n");
    while (list_isEmpty(favorites) == FALSE) {
        song = (Music *)list_popBack(favorites);
        if (!song || music_plain_print(stdout, song) < 0 || printf("\n") < 0) {
            radio_free(radio);
            list_free(favorites);
            fprintf(stderr, "ERROR: Could not extract from the back of the list\n");
            return ERROR_MAIN;
        }
    }

    list_free(favorites);
    radio_free(radio);
    return 0;
}

static int music_list_print(FILE *pf, const void *m) {
    return music_plain_print(pf, m);
}
