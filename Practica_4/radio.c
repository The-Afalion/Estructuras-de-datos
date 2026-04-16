/**
 * @file radio.c
 * @author Rodrigo y Rafael
 * @date March 2026
 * @version 1.0
 * @brief Implementation of the Radio TAD
 */

#include "radio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "stack.h"

#define MAX_MSC 4096

/**
 * @brief Internal radio representation.
 */
struct _Radio {
  Music *songs[MAX_MSC];
  Bool relations[MAX_MSC][MAX_MSC];
  int num_music;
  int num_relations;
};

/**
 * @brief Finds the array index of one song id.
 *
 * @param r Pointer to the radio.
 * @param id Song id.
 *
 * @return Array index if found, -1 otherwise.
 */
static int radio_findId(const Radio *r, long id) {
  int i;

  if (!r) {
    return -1;
  }

  for (i = 0; i < r->num_music; i++) {
    if (music_getId(r->songs[i]) == id) {
      return i;
    }
  }

  return -1;
}

/**
 * @brief Resets every song state before a graph traversal.
 *
 * @param r Pointer to the radio.
 *
 * @return OK on success, ERROR otherwise.
 */
static Status radio_resetStates(Radio *r) {
  int i;
  Music *current = NULL;

  if (!r) {
    return ERROR;
  }

  for (i = 0; i < r->num_music; i++) {
    current = radio_getMusicAt(r, i);
    if (!current || music_setState(current, NOT_LISTENED) == ERROR) {
      return ERROR;
    }
  }

  return OK;
}

/**
 * @brief Creates an empty radio.
 *
 * @return Pointer to the created radio, or NULL on error.
 */
Radio *radio_init() {
  Radio *r = NULL;
  int i, j;

  r = (Radio *)malloc(sizeof(Radio));
  if (!r) {
    return NULL;
  }

  r->num_music = 0;
  r->num_relations = 0;

  for (i = 0; i < MAX_MSC; i++) {
    r->songs[i] = NULL;
    for (j = 0; j < MAX_MSC; j++) {
      r->relations[i][j] = FALSE;
    }
  }

  return r;
}

/**
 * @brief Frees a radio and all its songs.
 *
 * @param r Pointer to the radio.
 */
void radio_free(Radio *r) {
  int i;

  if (!r) {
    return;
  }

  for (i = 0; i < r->num_music; i++) {
    music_free(r->songs[i]);
  }

  free(r);
}

/**
 * @brief Inserts one song into the radio.
 *
 * @param r Pointer to the radio.
 * @param desc Song description string.
 *
 * @return OK if the insertion succeeds, ERROR otherwise.
 */
Status radio_newMusic(Radio *r, char *desc) {
  Music *m = NULL;

  if (!r || !desc || r->num_music >= MAX_MSC) {
    return ERROR;
  }

  m = music_initFromString(desc);
  if (!m) {
    return ERROR;
  }

  if (radio_contains(r, music_getId(m)) == TRUE) {
    music_free(m);
    return OK;
  }

  if (music_setIndex(m, r->num_music) == ERROR) {
    music_free(m);
    return ERROR;
  }

  r->songs[r->num_music] = m;
  r->num_music++;

  return OK;
}

/**
 * @brief Inserts one relation into the radio.
 *
 * @param r Pointer to the radio.
 * @param orig Origin song id.
 * @param dest Destination song id.
 *
 * @return OK if the relation is added, ERROR otherwise.
 */
Status radio_newRelation(Radio *r, long orig, long dest) {
  int idx_orig, idx_dest;

  if (!r) {
    return ERROR;
  }

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

/**
 * @brief Checks whether one id exists in the radio.
 *
 * @param r Pointer to the radio.
 * @param id Song id.
 *
 * @return TRUE if the id exists, FALSE otherwise.
 */
Bool radio_contains(const Radio *r, long id) {
  if (!r) {
    return FALSE;
  }

  return (radio_findId(r, id) != -1) ? TRUE : FALSE;
}

/**
 * @brief Returns the number of songs in the radio.
 *
 * @param r Pointer to the radio.
 *
 * @return Number of songs, or -1 on error.
 */
int radio_getNumberOfMusic(const Radio *r) {
  if (!r) {
    return ERROR_I;
  }

  return r->num_music;
}

/**
 * @brief Returns the number of relations in the radio.
 *
 * @param r Pointer to the radio.
 *
 * @return Number of relations, or -1 on error.
 */
int radio_getNumberOfRelations(const Radio *r) {
  if (!r) {
    return ERROR_I;
  }

  return r->num_relations;
}

/**
 * @brief Checks whether one relation exists in the radio.
 *
 * @param r Pointer to the radio.
 * @param orig Origin song id.
 * @param dest Destination song id.
 *
 * @return TRUE if the relation exists, FALSE otherwise.
 */
Bool radio_relationExists(const Radio *r, long orig, long dest) {
  int idx_orig, idx_dest;

  if (!r) {
    return FALSE;
  }

  idx_orig = radio_findId(r, orig);
  idx_dest = radio_findId(r, dest);
  if (idx_orig == -1 || idx_dest == -1) {
    return FALSE;
  }

  return r->relations[idx_orig][idx_dest];
}

/**
 * @brief Returns the number of outgoing relations from one id.
 *
 * @param r Pointer to the radio.
 * @param id Song id.
 *
 * @return Number of outgoing relations, or -1 on error.
 */
int radio_getNumberOfRelationsFromId(const Radio *r, long id) {
  int idx, count, i;

  if (!r) {
    return ERROR_I;
  }

  idx = radio_findId(r, id);
  if (idx == -1) {
    return ERROR_I;
  }

  count = 0;
  for (i = 0; i < r->num_music; i++) {
    if (r->relations[idx][i] == TRUE) {
      count++;
    }
  }

  return count;
}

/**
 * @brief Returns an allocated array with the destination ids of one song.
 *
 * @param r Pointer to the radio.
 * @param id Song id.
 *
 * @return Allocated array, or NULL on error.
 */
long *radio_getRelationsFromId(const Radio *r, long id) {
  int idx, count, i, j;
  long *array = NULL;

  if (!r || id<0) {
    return NULL;
  }

  idx = radio_findId(r, id);
  if (idx == -1) {
    return NULL;
  }

  count = radio_getNumberOfRelationsFromId(r, id);
  if (count <= 0) {
    return NULL;
  }

  array = (long *)malloc((size_t)count * sizeof(long));
  if (!array) {
    return NULL;
  }

  j = 0;
  for (i = 0; i < r->num_music; i++) {
    if (r->relations[idx][i] == TRUE) {
      array[j] = music_getId(r->songs[i]);
      j++;
    }
  }

  return array;
}

/**
 * @brief Prints the radio.
 *
 * @param pf Output stream.
 * @param r Pointer to the radio.
 *
 * @return Number of written characters, or -1 on error.
 */
int radio_print(FILE *pf, const Radio *r) {
  int chars, i, j;

  if (!pf || !r) {
    return ERROR_I;
  }

  chars = 0;
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

/**
 * @brief Reads one radio definition from an input stream.
 *
 * @param fin Input stream.
 * @param r Pointer to the radio.
 *
 * @return OK if the radio is read correctly, ERROR otherwise.
 */
Status radio_readFromFile(FILE *fin, Radio *r) {
  char buff[STR_LENGTH * 5];
  int num_songs, i;
  long orig_id, dest_id;
  char extra;

  if (!fin || !r) {
    return ERROR;
  }
  //Init; nu music first line in file .txt
  if (fscanf(fin, "%d\n", &num_songs) != 1 || num_songs < 0 || num_songs > MAX_MSC) {
    return ERROR;
  }

  for (i = 0; i < num_songs; i++) {
    if (fgets(buff, sizeof(buff), fin) == NULL) {
      return ERROR;
    }

    buff[strcspn(buff, "\n")] = '\0';
    if (radio_newMusic(r, buff) == ERROR) {
      return ERROR;
    }
  }
  while (fgets(buff, sizeof(buff), fin) != NULL) {
    buff[strcspn(buff, "\r\n")] = '\0';
    if (buff[0] == '\0') {
      continue;
    }
    if (sscanf(buff, "%ld %ld %c", &orig_id, &dest_id, &extra) != 2) {
      return ERROR;
    }
    if (radio_newRelation(r, orig_id, dest_id) == ERROR) {
      return ERROR;
    }
  }

  return OK;
}

/**
 * @brief Returns the song stored at one array position.
 *
 * @param r Pointer to the radio.
 * @param index Array position.
 *
 * @return Pointer to the song, or NULL on error.
 */
Music *radio_getMusicAt(const Radio *r, int index) {
  if (!r || index < 0 || index >= r->num_music) {
    return NULL;
  }

  return r->songs[index];
}

/**
 * @brief Returns the song that matches one id.
 *
 * @param r Pointer to the radio.
 * @param id Song id.
 *
 * @return Pointer to the song, or NULL on error.
 */
Music *radio_getMusicById(const Radio *r, long id) {
  int idx;

  if (!r) {
    return NULL;
  }

  idx = radio_findId(r, id);
  if (idx == -1) {
    return NULL;
  }

  return r->songs[idx];
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
  int i, n_relations;

  if (!r) {
    return ERROR;
  }

  current = radio_getMusicById(r, from_id);
  if (!current || !radio_getMusicById(r, to_id)) {
    return ERROR;
  }

  if (radio_resetStates(r) == ERROR) {
    return ERROR;
  }

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

    if (music_plain_print(stdout, current) < 0 || printf("\n") < 0) {
      stack_free(pending);
      return ERROR;
    }

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

    for (i = 0; i < n_relations; i++) {
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

/**
 * @brief Runs an iterative BFS over the radio.
 *
 * The main behavioural difference between DFS and BFS is the pending-element
 * policy: DFS goes deep first because it uses a stack, while BFS explores by
 * levels because it uses a queue. BFS is usually a better fit for shortest-path
 * searches in unweighted graphs and for level-based exploration, for example:
 * recommendation systems or social-network degrees of separation. DFS usually
 * fits better for exhaustive exploration and backtracking, for example: maze
 * solving and dependency or file-tree traversal.
 *
 * @param r Pointer to the radio.
 * @param from_id Origin song id.
 * @param to_id Destination song id.
 *
 * @return OK if the destination is found, ERROR otherwise.
 */
Status radio_breadthSearch(Radio *r, long from_id, long to_id) {
  Queue *pending = NULL;
  Music *current = NULL;
  Music *next = NULL;
  long *relations = NULL;
  int i, n_relations;

  if (!r) return ERROR;

  current = radio_getMusicById(r, from_id);
  if (!current || !radio_getMusicById(r, to_id)) {
    return ERROR;
  }

  if (radio_resetStates(r) == ERROR) {
    return ERROR;
  }

  pending = queue_new();
  if (!pending) {
    return ERROR;
  }

  if (queue_push(pending, current) == ERROR) {
    queue_free(pending);
    return ERROR;
  }

  while (queue_isEmpty(pending) == FALSE) {
    current = (Music *)queue_pop(pending);
    if (!current) {
      queue_free(pending);
      return ERROR;
    }

    if (music_getState(current) == LISTENED) {
      continue;
    }

    if (music_setState(current, LISTENED) == ERROR) {
      queue_free(pending);
      return ERROR;
    }

    if (music_plain_print(stdout, current) < 0 || printf("\n") < 0) {
      queue_free(pending);
      return ERROR;
    }

    if (music_getId(current) == to_id) {
      queue_free(pending);
      return OK;
    }

    n_relations = radio_getNumberOfRelationsFromId(r, music_getId(current));
    if (n_relations <= 0) {
      continue;
    }

    relations = radio_getRelationsFromId(r, music_getId(current));
    if (!relations) {
      queue_free(pending);
      return ERROR;
    }

    for (i = 0; i < n_relations; i++) {
      next = radio_getMusicById(r, relations[i]);
      if (next && music_getState(next) == NOT_LISTENED) {
        if (queue_push(pending, next) == ERROR) {
          free(relations);
          queue_free(pending);
          return ERROR;
        }
      }
    }

    free(relations);
    relations = NULL;
  }

  queue_free(pending);
  return ERROR;
}
