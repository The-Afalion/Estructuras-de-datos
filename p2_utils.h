#ifndef P2_UTILS_H
#define P2_UTILS_H

#include <stdio.h>

#include "music.h"
#include "radio.h"
#include "stack.h"
#include "types.h"

/**
 * @brief Function pointer type used to compare two stack elements.
 *
 * @param a First element.
 * @param b Second element.
 *
 * @return A negative value if a < b, zero if a == b, and a positive value if a > b.
 */
typedef int (*P_stack_ele_cmp)(const void *a, const void *b);

/**
 * @brief Merges two ordered input stacks into one ordered output stack.
 *
 * The input stacks must store elements in descending order at the top.
 * The output stack is filled so that it remains ordered in ascending order.
 *
 * @param sin1 First input stack.
 * @param sin2 Second input stack.
 * @param sout Output stack.
 * @param cmp Comparison function.
 *
 * @return OK if the merge finishes correctly, ERROR otherwise.
 */
Status mergeStacks(Stack *sin1, Stack *sin2, Stack *sout, P_stack_ele_cmp cmp);

/**
 * @brief Compares two songs by duration.
 *
 * @param a First song.
 * @param b Second song.
 *
 * @return Standard comparison result.
 */
int music_duration_cmp(const void *a, const void *b);

/**
 * @brief Compares two songs by artist.
 *
 * @param a First song.
 * @param b Second song.
 *
 * @return Standard comparison result.
 */
int music_artist_cmp(const void *a, const void *b);

/**
 * @brief Prints one song using a signature compatible with stack_print.
 *
 * @param pf Output stream.
 * @param m Song to print.
 *
 * @return Number of written characters, or -1 on error.
 */
int music_stack_print(FILE *pf, const void *m);

/**
 * @brief Loads a radio from a file and pushes all its songs into a stack.
 *
 * @param filename Input file name.
 * @param r Output radio pointer.
 * @param s Output stack pointer.
 *
 * @return OK if everything works, ERROR otherwise.
 */
Status load_radio_stack(const char *filename, Radio **r, Stack **s);

/**
 * @brief Runs an iterative DFS over the radio.
 *
 * The function prints each visited song while exploring recommendations.
 *
 * @param r Pointer to the radio.
 * @param from_id Origin song id.
 * @param to_id Destination song id.
 *
 * @return OK if the destination is found, ERROR otherwise.
 */
Status radio_depthSearch(Radio *r, long from_id, long to_id);

#endif
