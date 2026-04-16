/**
 * @file  music.h
 * @author Profesores EDAT
 * @date February 2026
 * @version 1.0
 * @brief Library to manage ADT Music
 */

#ifndef MUSIC_H
#define MUSIC_H

#include <stdio.h>

#include "types.h"


typedef enum { NOT_LISTENED = 0, LISTENED, ERROR_MUSIC } State;

typedef struct _Music Music;

/**
 * @brief Creates an empty song.
 *
 * @return Pointer to the created song, or NULL on error.
 */
Music* music_init();

/**
 * @brief Creates a song from a text description.
 *
 * @param descr Description string.
 *
 * @return Pointer to the created song, or NULL on error.
 */
Music* music_initFromString(char* descr);

/**
 * @brief Frees one song.
 *
 * @param m Pointer to the song.
 */
void music_free(void* m);

/**
 * @brief Returns the song id.
 *
 * @param m Pointer to the song.
 *
 * @return Song id, or ERROR_I on error.
 */
long music_getId(const Music* m);

/**
 * @brief Returns the song title.
 *
 * @param m Pointer to the song.
 *
 * @return Pointer to the title, or NULL on error.
 */
const char* music_getTitle(const Music* m);

/**
 * @brief Returns the song artist.
 *
 * @param m Pointer to the song.
 *
 * @return Pointer to the artist, or NULL on error.
 */
const char* music_getArtist(const Music* m);

/**
 * @brief Returns the song duration.
 *
 * @param m Pointer to the song.
 *
 * @return Song duration, or 0 on error.
 */
unsigned short music_getDuration(const Music* m);

/**
 * @brief Returns the song state.
 *
 * @param m Pointer to the song.
 *
 * @return Song state, or ERROR_MUSIC on error.
 */
State music_getState(const Music* m);

/**
 * @brief Returns the song index inside the radio array.
 *
 * @param m Pointer to the song.
 *
 * @return Song index, or ERROR_I on error.
 */
int music_getIndex(const Music* m);

/**
 * @brief Sets the song id.
 *
 * @param m Pointer to the song.
 * @param id New id.
 *
 * @return OK on success, ERROR otherwise.
 */
Status music_setId(Music* m, const long id);

/**
 * @brief Sets the song title.
 *
 * @param m Pointer to the song.
 * @param title New title.
 *
 * @return OK on success, ERROR otherwise.
 */
Status music_setTitle(Music* m, const char* title);

/**
 * @brief Sets the song artist.
 *
 * @param m Pointer to the song.
 * @param artist New artist.
 *
 * @return OK on success, ERROR otherwise.
 */
Status music_setArtist(Music* m, const char* artist);

/**
 * @brief Sets the song duration.
 *
 * @param m Pointer to the song.
 * @param duration New duration.
 *
 * @return OK on success, ERROR otherwise.
 */
Status music_setDuration(Music* m, const unsigned short duration);

/**
 * @brief Sets the song state.
 *
 * @param m Pointer to the song.
 * @param state New state.
 *
 * @return OK on success, ERROR otherwise.
 */
Status music_setState(Music* m, const State state);

/**
 * @brief Sets the song index inside the radio array.
 *
 * @param m Pointer to the song.
 * @param index New index.
 *
 * @return OK on success, ERROR otherwise.
 */
Status music_setIndex(Music* m, int index);

/**
 * @brief Compares two songs.
 *
 * @param m1 First song.
 * @param m2 Second song.
 *
 * @return Standard comparison result.
 */
int music_cmp(const void* m1, const void* m2);

/**
 * @brief Creates a heap copy of one song.
 *
 * @param src Source song.
 *
 * @return Pointer to the copied song, or NULL on error.
 */
void* music_copy(const void* src);

/**
 * @brief Prints one song in plain format.
 *
 * @param pf Output stream.
 * @param m Song to print.
 * @param connections Number of connections to print.
 *
 * @return Number of written characters, or ERROR_I on error.
 */
int music_plain_print(FILE* pf, const void* m);

/**
 * @brief Prints one song in player format.
 *
 * @param pf Output stream.
 * @param m Song to print.
 *
 * @return Number of written characters, or -1 on error.
 */
int music_formatted_print(FILE* pf, const void* m);

#endif
