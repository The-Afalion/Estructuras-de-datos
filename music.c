/**
 * @file  music.c
 * @author Profesores EDAT
 * @date February 2026
 * @version 1.0
 * @brief Library to manage ADT Music
 */

#include "music.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Internal song representation.
 */
struct _Music {
    long id;
    char title[STR_LENGTH];
    char artist[STR_LENGTH];
    unsigned short duration;
    State status;
    int index;
};

/**
 * @brief Assigns one parsed field to a song.
 *
 * @param m Pointer to the song.
 * @param key Parsed key.
 * @param value Parsed value.
 *
 * @return OK on success, ERROR otherwise.
 */
static Status music_setField(Music* m, char* key, char* value) {
    if (!m || !key || !value) {
        return ERROR;
    }

    if (strcmp(key, "id") == 0 || strcmp(key, "iid") == 0) {
        return music_setId(m, atol(value));
    }
    if (strcmp(key, "title") == 0) {
        return music_setTitle(m, value);
    }
    if (strcmp(key, "artist") == 0) {
        return music_setArtist(m, value);
    }
    if (strcmp(key, "duration") == 0) {
        return music_setDuration(m, (unsigned short)atoi(value));
    }
    if (strcmp(key, "state") == 0) {
        return music_setState(m, (State)atoi(value));
    }

    return ERROR;
}

/**
 * @brief Creates an empty song.
 *
 * @return Pointer to the created song, or NULL on error.
 */
Music* music_init() {
    Music* newMusic = NULL;

    newMusic = (Music*)malloc(sizeof(Music));
    if (!newMusic) {
        return NULL;
    }

    newMusic->id = 0;
    strcpy(newMusic->title, "");
    strcpy(newMusic->artist, "");
    newMusic->duration = 0;
    newMusic->status = NOT_LISTENED;
    newMusic->index = -1;

    return newMusic;
}

/**
 * @brief Creates a song from a text description.
 *
 * @param descr Description string.
 *
 * @return Pointer to the created song, or NULL on error.
 */
Music* music_initFromString(char* descr) {
    Music* m = NULL;
    char *buffer = NULL, *p = NULL, *key_start = NULL, *value_start = NULL;

    if (!descr) {
        return NULL;
    }

    buffer = strdup(descr);
    if (!buffer) {
        return NULL;
    }

    m = music_init();
    if (!m) {
        free(buffer);
        return NULL;
    }

    p = buffer;
    while (*p) {
        while (isspace((unsigned char)*p)) {
            p++;
        }
        if (!*p) {
            break;
        }

        key_start = p;
        while (*p && *p != ':') {
            p++;
        }
        if (!*p) {
            break;
        }
        *p = '\0';
        p++;

        while (isspace((unsigned char)*p)) {
            p++;
        }
        if (*p != '"') {
            break;
        }
        p++;

        value_start = p;
        while (*p && *p != '"') {
            p++;
        }
        if (!*p) {
            break;
        }
        *p = '\0';
        p++;

        music_setField(m, key_start, value_start);
    }

    free(buffer);
    return m;
}

/**
 * @brief Frees one song.
 *
 * @param m Pointer to the song.
 */
void music_free(void* m) {
    if (m) {
        free(m);
    }
}

/**
 * @brief Returns the song id.
 *
 * @param m Pointer to the song.
 *
 * @return Song id, or ERROR_I on error.
 */
long music_getId(const Music* m) {
    if (!m) return ERROR_I;

    return m->id;
}

/**
 * @brief Returns the song title.
 *
 * @param m Pointer to the song.
 *
 * @return Pointer to the title, or NULL on error.
 */
const char* music_getTitle(const Music* m) {
    if (!m) {
        return NULL;
    }

    return m->title;
}

/**
 * @brief Returns the song artist.
 *
 * @param m Pointer to the song.
 *
 * @return Pointer to the artist, or NULL on error.
 */
const char* music_getArtist(const Music* m) {
    if (!m) {
        return NULL;
    }

    return m->artist;
}

/**
 * @brief Returns the song duration.
 *
 * @param m Pointer to the song.
 *
 * @return Song duration, or 0 on error.
 */
unsigned short music_getDuration(const Music* m) {
    if (!m) return 0;

    return m->duration;
}

/**
 * @brief Returns the song state.
 *
 * @param m Pointer to the song.
 *
 * @return Song state, or ERROR_MUSIC on error.
 */
State music_getState(const Music* m) {
    if (!m) return ERROR_MUSIC;

    return m->status;
}

/**
 * @brief Returns the song index.
 *
 * @param m Pointer to the song.
 *
 * @return Song index, or ERROR_I on error.
 */
int music_getIndex(const Music* m) {
    if (!m) return ERROR_I;

    return m->index;
}

/**
 * @brief Sets the song id.
 *
 * @param m Pointer to the song.
 * @param id New id.
 *
 * @return OK on success, ERROR otherwise.
 */
Status music_setId(Music* m, const long id) {
    if (!m)return ERROR;

    m->id = id;
    return OK;
}

/**
 * @brief Sets the song title.
 *
 * @param m Pointer to the song.
 * @param title New title.
 *
 * @return OK on success, ERROR otherwise.
 */
Status music_setTitle(Music* m, const char* title) {
    if (!m || !title) return ERROR;

    strncpy(m->title, title, STR_LENGTH - 1);
    m->title[STR_LENGTH - 1] = '\0';
    return OK;
}

/**
 * @brief Sets the song artist.
 *
 * @param m Pointer to the song.
 * @param artist New artist.
 *
 * @return OK on success, ERROR otherwise.
 */
Status music_setArtist(Music* m, const char* artist) {
    if (!m || !artist) return ERROR;

    strncpy(m->artist, artist, STR_LENGTH - 1);
    m->artist[STR_LENGTH - 1] = '\0';
    return OK;
}

/**
 * @brief Sets the song duration.
 *
 * @param m Pointer to the song.
 * @param duration New duration.
 *
 * @return OK on success, ERROR otherwise.
 */
Status music_setDuration(Music* m, const unsigned short duration) {
    if (!m) return ERROR;

    m->duration = duration;
    return OK;
}

/**
 * @brief Sets the song state.
 *
 * @param m Pointer to the song.
 * @param state New state.
 *
 * @return OK on success, ERROR otherwise.
 */
Status music_setState(Music* m, const State state) {
    if (!m) {
        return ERROR;
    }

    m->status = state;
    return OK;
}

/**
 * @brief Sets the song index.
 *
 * @param m Pointer to the song.
 * @param index New index.
 *
 * @return OK on success, ERROR otherwise.
 */
Status music_setIndex(Music* m, int index) {
    if (!m || index < 0) {
        return ERROR;
    }

    m->index = index;
    return OK;
}

/**
 * @brief Compares two songs.
 *
 * @param m1 First song.
 * @param m2 Second song.
 *
 * @return Standard comparison result.
 */
int music_cmp(const void* m1, const void* m2) {
    const Music* music1 = NULL;
    const Music* music2 = NULL;
    int result;

    if (!m1 || !m2) {
        return ERROR_I;
    }

    music1 = (const Music*)m1;
    music2 = (const Music*)m2;

    if (music1->id < music2->id) {
        return -1;
    }
    if (music1->id > music2->id) {
        return 1;
    }

    result = strcmp(music1->title, music2->title);
    if (result != 0) {
        return result;
    }

    return strcmp(music1->artist, music2->artist);
}

/**
 * @brief Creates a heap copy of one song.
 *
 * @param src Source song.
 *
 * @return Pointer to the copied song, or NULL on error.
 */
void* music_copy(const void* src) {
    const Music* source = NULL;
    Music* dest = NULL;

    if (!src) {
        return NULL;
    }

    source = (const Music*)src;
    dest = music_init();
    if (!dest) {
        return NULL;
    }

    *dest = *source;
    return dest;
}

/**
 * @brief Prints one song in plain format.
 *
 * @param pf Output stream.
 * @param m Song to print.
 * @param connections Number of connections.
 *
 * @return Number of written characters, or ERROR_I on error.
 */
int music_plain_print(FILE* pf, const void* m) {
    const Music* music = NULL;

    if (!pf || !m) {
        return ERROR_I;
    }

    music = (const Music*)m;
    return fprintf(pf, "[%ld, %s, %s, %hu, %d]", music->id, music->title,
                   music->artist, music->duration, music->index);
}

/**
 * @brief Prints one song in player format.
 *
 * @param pf Output stream.
 * @param m Song to print.
 *
 * @return Number of written characters, or -1 on error.
 */
int music_formatted_print(FILE* pf, const void* m) {
    const Music* aux = NULL;
    int counter, minutes, sec;

    if (!pf || !m) {
        return -1;
    }

    aux = (const Music*)m;
    if (aux->duration == 0) {
        return -1;
    }

    minutes = aux->duration / 60;
    sec = aux->duration % 60;

    counter = fprintf(pf, "\t ɴᴏᴡ ᴘʟᴀʏɪɴɢ: %s\n", aux->title);
    counter += fprintf(pf, "\t • Artist %s •\n", aux->artist);
    counter += fprintf(pf, "\t──────────⚪──────────\n");
    counter += fprintf(pf, "\t\t◄◄⠀▐▐ ⠀►►\n");
    counter += fprintf(pf, "\t 0:00 / %02d:%02d ───○ 🔊⠀\n\n", minutes, sec);

    return counter;
}
