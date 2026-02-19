/**
 * @file radio.c
 * @author Rodrigo y Rafael
 * @date March 2024
 * @version 1.0
 * @brief Implementation of the Radio TAD
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "radio.h"

#define MAX_MSC 4096

/**
 * @brief Radio structure
 */
struct _Radio {
    Music* songs[MAX_MSC];
    Bool relations[MAX_MSC][MAX_MSC];
    int num_music;
    int num_relations;
};

/* Private function to find the index of a music by its ID */
int radio_findId(const Radio *r, long id) {
    int i;
    if (!r) return -1;
    for (i = 0; i < r->num_music; i++) {
        if (music_getId(r->songs[i]) == id) {
            return i;
        }
    }
    return -1; /* Not found */
}

Radio * radio_init() {
    Radio *r = (Radio *) malloc(sizeof(Radio));
    if (!r) return NULL;

    r->num_music = 0;
    r->num_relations = 0;

    for (int i = 0; i < MAX_MSC; i++) {
        r->songs[i] = NULL;
        for (int j = 0; j < MAX_MSC; j++) {
            r->relations[i][j] = FALSE;
        }
    }
    return r;
}

void radio_free(Radio *r) {
    if (!r) return;
    for (int i = 0; i < r->num_music; i++) {
        music_free(r->songs[i]);
    }
    free(r);
}

Status radio_newMusic(Radio *r, char *desc) {
    Music *m;
    if (!r || !desc) return ERROR;
    if (r->num_music >= MAX_MSC) return ERROR;

    m = music_initFromString(desc);
    if (!m) return ERROR;

    if (radio_contains(r, music_getId(m)) == TRUE) {
        music_free(m); /* Already exists, free the new one */
        return OK; /* As per header, OK if it already exists */
    }

    r->songs[r->num_music] = m;
    r->num_music++;

    return OK;
}

Status radio_newRelation(Radio *r, long orig, long dest) {
    int idx_orig, idx_dest;
    if (!r) return ERROR;

    idx_orig = radio_findId(r, orig);
    idx_dest = radio_findId(r, dest);

    if (idx_orig == -1 || idx_dest == -1) {
        return ERROR;
    }

    if (r->relations[idx_orig][idx_dest] == FALSE) {
        r->relations[idx_orig][idx_dest] = TRUE;
        r->num_relations++;
    }

    return OK;
}

Bool radio_contains(const Radio *r, long id) {
    if (!r) return FALSE;
    return (radio_findId(r, id) != -1);
}

int radio_getNumberOfMusic(const Radio *r) {
    if (!r) return -1;
    return r->num_music;
}

int radio_getNumberOfRelations(const Radio *r) {
    if (!r) return -1;
    return r->num_relations;
}

Bool radio_relationExists(const Radio *r, long orig, long dest) {
    int idx_orig, idx_dest;
    if (!r) return FALSE;

    idx_orig = radio_findId(r, orig);
    idx_dest = radio_findId(r, dest);

    if (idx_orig == -1 || idx_dest == -1) {
        return FALSE;
    }

    return r->relations[idx_orig][idx_dest];
}

int radio_print(FILE *pf, const Radio *r) {
    int chars = 0, i, j;
    if (!pf || !r) return -1;

    for (i = 0; i < r->num_music; i++) {
        chars += music_plain_print(pf, r->songs[i]);
        chars += fprintf(pf, ":");

        for (j = 0; j < r->num_music; j++) {
            if (r->relations[i][j] == TRUE) {
                chars += fprintf(pf, " ");
                chars += music_plain_print(pf, r->songs[j]);
            }
        }
        chars += fprintf(pf, "\n");
    }
    return chars;
}

Status radio_readFromFile(FILE *fin, Radio *r) {
    char buff[STR_LENGTH * 5];
    int num_songs, i;
    long orig_id, dest_id;
    char c;

    if (!fin || !r) return ERROR;

    /* Read number of songs */
    if (fscanf(fin, "%d\n", &num_songs) != 1) return ERROR;

    /* Read songs descriptions */
    for (i = 0; i < num_songs; i++) {
        if (fgets(buff, sizeof(buff), fin) == NULL) return ERROR;
        /* Remove trailing newline */
        buff[strcspn(buff, "\n")] = 0;
        if (radio_newMusic(r, buff) == ERROR) return ERROR;
    }

    /* Read relations */
    while (fscanf(fin, "%ld", &orig_id) == 1) {
        /* Read destination IDs on the same line */
        while ((c = fgetc(fin)) != '\n' && c != EOF) {
            if (ungetc(c, fin) == EOF) break; /* Put character back */
            if (fscanf(fin, "%ld", &dest_id) == 1) {
                radio_newRelation(r, orig_id, dest_id);
            } else {
                /* Could not read a long, skip to next line */
                while((c = fgetc(fin)) != '\n' && c != EOF);
                break;
            }
        }
    }

    return OK;
}

/* Extra functions from radio.h that are not part of the exercise but needed to compile */
int radio_getNumberOfRelationsFromId(const Radio *r, long id) {
    int idx, count = 0;
    if (!r) return -1;
    idx = radio_findId(r, id);
    if (idx == -1) return -1;

    for (int i = 0; i < r->num_music; i++) {
        if (r->relations[idx][i] == TRUE) {
            count++;
        }
    }
    return count;
}

long *radio_getRelationsFromId(const Radio *r, long id) {
    int idx, count, i, j = 0;
    long *array;

    if (!r) return NULL;
    idx = radio_findId(r, id);
    if (idx == -1) return NULL;

    count = radio_getNumberOfRelationsFromId(r, id);
    if (count <= 0) return NULL;

    array = (long *) malloc(count * sizeof(long));
    if (!array) return NULL;

    for (i = 0; i < r->num_music; i++) {
        if (r->relations[idx][i] == TRUE) {
            array[j] = music_getId(r->songs[i]);
            j++;
        }
    }
    return array;
}
