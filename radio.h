/**
 * @file radio.h
 * @author Profesores EDAT
 * @date February 2026
 * @version 1.0
 * @brief Library to manage the radio TAD
 */

#ifndef RADIO_H
#define RADIO_H

#include "music.h"

typedef struct _Radio Radio;

/**
 * @brief Creates a new empty radio.
 *
 * @return A pointer to the radio if it was correctly allocated, NULL otherwise.
 */
Radio* radio_init();

/**
 * @brief Frees a radio.
 *
 * @param r Pointer to the radio.
 */
void radio_free(Radio* r);

/**
 * @brief Inserts a new song into the radio.
 *
 * @param r Pointer to the radio.
 * @param desc Song description string.
 *
 * @return OK if the song is inserted correctly, ERROR otherwise.
 */
Status radio_newMusic(Radio* r, char* desc);

/**
 * @brief Creates a relation between two songs.
 *
 * @param r Pointer to the radio.
 * @param orig Origin song id.
 * @param dest Destination song id.
 *
 * @return OK if the relation is inserted correctly, ERROR otherwise.
 */
Status radio_newRelation(Radio* r, long orig, long dest);

/**
 * @brief Checks whether a song exists in the radio.
 *
 * @param r Pointer to the radio.
 * @param id Song id.
 *
 * @return TRUE if the song exists, FALSE otherwise.
 */
Bool radio_contains(const Radio* r, long id);

/**
 * @brief Returns the number of songs in the radio.
 *
 * @param r Pointer to the radio.
 *
 * @return Number of songs, or -1 on error.
 */
int radio_getNumberOfMusic(const Radio* r);

/**
 * @brief Returns the number of relations in the radio.
 *
 * @param r Pointer to the radio.
 *
 * @return Number of relations, or -1 on error.
 */
int radio_getNumberOfRelations(const Radio* r);

/**
 * @brief Checks whether a relation exists between two songs.
 *
 * @param r Pointer to the radio.
 * @param orig Origin song id.
 * @param dest Destination song id.
 *
 * @return TRUE if the relation exists, FALSE otherwise.
 */
Bool radio_relationExists(const Radio* r, long orig, long dest);

/**
 * @brief Returns the number of outgoing relations from one song.
 *
 * @param r Pointer to the radio.
 * @param id Origin song id.
 *
 * @return Number of outgoing relations, or -1 on error.
 */
int radio_getNumberOfRelationsFromId(const Radio* r, long id);

/**
 * @brief Returns an allocated array with the destination ids of one song.
 *
 * @param r Pointer to the radio.
 * @param id Origin song id.
 *
 * @return Allocated array with destination ids, or NULL on error.
 */
long* radio_getRelationsFromId(const Radio* r, long id);

/**
 * @brief Prints the whole radio.
 *
 * @param pf Output stream.
 * @param r Pointer to the radio.
 *
 * @return Number of written characters, or -1 on error.
 */
int radio_print(FILE* pf, const Radio* r);

/**
 * @brief Reads a radio definition from a text file.
 *
 * @param fin Input stream.
 * @param r Pointer to the radio.
 *
 * @return OK if the radio is read correctly, ERROR otherwise.
 */
Status radio_readFromFile(FILE* fin, Radio* r);

/**
 * @brief Returns the song stored at one array position.
 *
 * @param r Pointer to the radio.
 * @param index Array position.
 *
 * @return Pointer to the song, or NULL on error.
 */
Music* radio_getMusicAt(const Radio* r, int index);

/**
 * @brief Returns the song that matches one id.
 *
 * @param r Pointer to the radio.
 * @param id Song id.
 *
 * @return Pointer to the song, or NULL on error.
 */
Music* radio_getMusicById(const Radio* r, long id);

#endif
