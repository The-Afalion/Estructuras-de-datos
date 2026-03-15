#include "p2_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Merges two ordered stacks into one ordered result stack.
 *
 * @param sin1 First input stack.
 * @param sin2 Second input stack.
 * @param sout Output stack.
 * @param cmp Comparison function.
 *
 * @return OK if the merge succeeds, ERROR otherwise.
 */
Status mergeStacks(Stack *sin1, Stack *sin2, Stack *sout, P_stack_ele_cmp cmp) {
  const void *top1 = NULL;
  const void *top2 = NULL;

  if (!sin1 || !sin2 || !sout || !cmp) {
    return ERROR;
  }

  while (stack_isEmpty(sin1) == FALSE && stack_isEmpty(sin2) == FALSE) {
    top1 = stack_top(sin1);
    top2 = stack_top(sin2);

    if (!top1 || !top2) {
      return ERROR;
    }

    if (cmp(top1, top2) >= 0) {
      if (stack_push(sout, stack_pop(sin1)) == ERROR) {
        return ERROR;
      }
    } else {
      if (stack_push(sout, stack_pop(sin2)) == ERROR) {
        return ERROR;
      }
    }
  }

  while (stack_isEmpty(sin1) == FALSE) {
    if (stack_push(sout, stack_pop(sin1)) == ERROR) {
      return ERROR;
    }
  }

  while (stack_isEmpty(sin2) == FALSE) {
    if (stack_push(sout, stack_pop(sin2)) == ERROR) {
      return ERROR;
    }
  }

  return OK;
}

/**
 * @brief Compares two songs by duration.
 *
 * @param a First song.
 * @param b Second song.
 *
 * @return Standard comparison result.
 */
int music_duration_cmp(const void *a, const void *b) {
  const Music *m1 = NULL;
  const Music *m2 = NULL;
  unsigned short d1, d2;

  if (!a || !b)
    return 0;

  m1 = (const Music *)a;
  m2 = (const Music *)b;
  d1 = music_getDuration(m1);
  d2 = music_getDuration(m2);

  if (d1 < d2)
    return -1;

  if (d1 > d2)
    return 1;

  return music_cmp(a, b);
}

/**
 * @brief Compares two songs by artist.
 *
 * @param a First song.
 * @param b Second song.
 *
 * @return Standard comparison result.
 */
int music_artist_cmp(const void *a, const void *b) {
  const Music *m1 = NULL;
  const Music *m2 = NULL;
  int cmp;

  if (!a || !b) {
    return 0;
  }

  m1 = (const Music *)a;
  m2 = (const Music *)b;

  cmp = strcmp(music_getArtist(m1), music_getArtist(m2));
  if (cmp != 0) {
    return cmp;
  }

  cmp = strcmp(music_getTitle(m1), music_getTitle(m2));
  if (cmp != 0) {
    return cmp;
  }

  return music_cmp(a, b);
}

/**
 * @brief Prints one song with the signature required by stack_print.
 *
 * @param pf Output stream.
 * @param m Song to print.
 *
 * @return Number of written characters, or -1 on error.
 */
int music_stack_print(FILE *pf, const void *m) {
  if (!pf || !m) {
    return -1;
  }

  return music_plain_print(pf, m);
}

/**
 * @brief Loads a radio from a file and fills a stack with its songs.
 *
 * @param filename Input file name.
 * @param r Output radio pointer.
 * @param s Output stack pointer.
 *
 * @return OK if everything works, ERROR otherwise.
 */
Status load_radio_stack(const char *filename, Radio **r, Stack **s) {
  FILE *fin = NULL;
  int i, n_music;
  Music *m = NULL;

  if (!filename || !r || !s) {
    return ERROR;
  }

  fin = fopen(filename, "r");
  if (!fin) {
    return ERROR;
  }

  *r = radio_init();
  *s = stack_init();
  if (!*r || !*s) {
    fclose(fin);
    radio_free(*r);
    stack_free(*s);
    *r = NULL;
    *s = NULL;
    return ERROR;
  }

  if (radio_readFromFile(fin, *r) == ERROR) {
    fclose(fin);
    radio_free(*r);
    stack_free(*s);
    *r = NULL;
    *s = NULL;
    return ERROR;
  }
  fclose(fin);

  n_music = radio_getNumberOfMusic(*r);
  for (i = 0; i < n_music; i++) {
    m = radio_getMusicAt(*r, i);
    if (!m || stack_push(*s, m) == ERROR) {
      radio_free(*r);
      stack_free(*s);
      *r = NULL;
      *s = NULL;
      return ERROR;
    }
  }

  return OK;
}

/**
 * @brief Runs an iterative DFS over the radio.
 *
 * @param r Pointer to the radio.
 * @param from_id Origin song id.
 * @param to_id Destination song id.
 *
 * @return OK if the destination is found, ERROR otherwise.
 */
Status radio_depthSearch(Radio *r, long from_id, long to_id) {
  Stack *pending = NULL;
  Music *current = NULL;
  Music *next = NULL;
  long *relations = NULL;
  int i, n_relations, n_music;

  if (!r) {
    return ERROR;
  }

  current = radio_getMusicById(r, from_id);
  if (!current || !radio_getMusicById(r, to_id)) {
    return ERROR;
  }

  n_music = radio_getNumberOfMusic(r);
  for (i = 0; i < n_music; i++) {
    current = radio_getMusicAt(r, i);
    if (!current || music_setState(current, NOT_LISTENED) == ERROR) {
      return ERROR;
    }
  }

  current = radio_getMusicById(r, from_id);
  pending = stack_init();
  if (!pending) {
    return ERROR;
  }

  if (stack_push(pending, current) == ERROR) {
    stack_free(pending);
    return ERROR;
  }

  while (stack_isEmpty(pending) == FALSE) {
    current = (Music *)stack_pop(pending);
    if (!current) {
      stack_free(pending);
      return ERROR;
    }

    if (music_getState(current) == LISTENED) {
      continue;
    }

    if (music_setState(current, LISTENED) == ERROR) {
      stack_free(pending);
      return ERROR;
    }

    music_plain_print(stdout, current);
    printf("\n");

    if (music_getId(current) == to_id) {
      stack_free(pending);
      return OK;
    }

    n_relations = radio_getNumberOfRelationsFromId(r, music_getId(current));
    if (n_relations <= 0) {
      continue;
    }

    relations = radio_getRelationsFromId(r, music_getId(current));
    if (!relations) {
      stack_free(pending);
      return ERROR;
    }

    for (i = n_relations - 1; i >= 0; i--) {
      next = radio_getMusicById(r, relations[i]);
      if (next && music_getState(next) == NOT_LISTENED) {
        if (stack_push(pending, next) == ERROR) {
          free(relations);
          stack_free(pending);
          return ERROR;
        }
      }
    }

    free(relations);
    relations = NULL;
  }

  stack_free(pending);
  return ERROR;
}
