/**
 * @file p3_e1.c
 * @brief Exercise 1: Playback queue simulation.
 */

#include <stdio.h>
#include <stdlib.h>

#include "music.h"
#include "radio.h"
#include "queue.h"

#define ERROR_MAIN 1
#define MIN_ARGC 2

static int music_queue_print(FILE *pf, const void *m);
static int now_playing_menu(Queue *q);

int main(int argc, char *argv[]) {
    FILE *fin = NULL;
    Radio *r = NULL;
    Queue *q = NULL;
    Music *song = NULL;
    int i, num_music;

    if (argc < MIN_ARGC) {
        fprintf(stderr, "Usage: %s <radio.txt>\n", argv[0]);
        return ERROR_MAIN;
    }

    fin = fopen(argv[1], "r");
    if (!fin) {
        fprintf(stderr, "ERROR: Could not open input file\n");
        return ERROR_MAIN;
    }

    r = radio_init();
    q = queue_new();
    if (!r || !q) {
        fclose(fin);
        radio_free(r);
        queue_free(q);
        fprintf(stderr, "ERROR: Could not create radio or queue\n");
        return ERROR_MAIN;
    }

    if (radio_readFromFile(fin, r) == ERROR) {
        fclose(fin);
        radio_free(r);
        queue_free(q);
        fprintf(stderr, "ERROR: Could not read the radio\n");
        return ERROR_MAIN;
    }
    fclose(fin);

    num_music = radio_getNumberOfMusic(r);
    if (num_music == ERROR_I) {
        radio_free(r);
        queue_free(q);
        fprintf(stderr, "ERROR: Could not get the number of songs\n");
        return ERROR_MAIN;
    }

    for (i = 0; i < num_music; i++) {
        song = radio_getMusicAt(r, i);
        if (!song || queue_push(q, song) == ERROR) {
            radio_free(r);
            queue_free(q);
            fprintf(stderr, "ERROR: Could not fill the playback queue\n");
            return ERROR_MAIN;
        }
    }

    now_playing_menu(q);

    queue_free(q);
    radio_free(r);

    return 0;
}

static int music_queue_print(FILE *pf, const void *m) {
    return music_plain_print(pf, m);
}

/**
 * @brief Interactive menu that simulates a music player.
 *
 * Pops the front song as "now playing", shows the remaining queue as
 * "Upcoming", and lets the user advance or exit.
 *
 * @param q Playback queue.
 *
 * @return Last selected option.
 */
static int now_playing_menu(Queue *q) {
    char buffer[32];
    int option = 0;
    Music *current = NULL;

    if (!q) {
        return 2;
    }

    do {
        if (queue_isEmpty(q) == FALSE) {
            /* Pop the current song to display it */
            current = (Music *)queue_pop(q);
            music_formatted_print(stdout, current);

            /* Show remaining songs as upcoming */
            printf("Upcoming:\n");
            queue_print(stdout, q, music_queue_print);
        } else {
            printf("\nNo song currently playing.\n\n");
        }

        printf("\n1. Next song\n");
        printf("2. Exit\n");
        printf("Choose an option: ");

        if (!fgets(buffer, sizeof(buffer), stdin)) {
            return 2;
        }

        option = atoi(buffer);

        /* Option 1: loop naturally pops the next song.
         * Option 2 or empty queue: exit. */
    } while (option != 2 && queue_isEmpty(q) == FALSE);

    return option;
}
