/**
 * @file  p4_e2.c
 * @author Rafael
 * @date May 2026
 * @version 1.0
 * @brief Exercise 3 of Practice 4: tree_rangeSearch test program.
 *
 * Reads a radio file, builds a BST of songs (ordered by music_cmp, i.e. by id),
 * and asks the tree for every song whose id is in the range [min_id, max_id].
 * Finally, the resulting list is printed in order.
 */

#include <stdio.h>
#include <stdlib.h>

#include "bstree.h"
#include "list.h"
#include "music.h"
#include "radio.h"
#include "types.h"

#define ERROR_MAIN 1
#define MIN_ARGC 4

/**
 * @brief Frees every owned resource and exits with the given code.
 */
static void mainCleanUp(int ret_value, BSTree *tree, Radio *radio,
                        Music *min_music, Music *max_music, FILE *pf) {
  if (tree) tree_destroy(tree);
  if (radio) radio_free(radio);
  if (min_music) music_free(min_music);
  if (max_music) music_free(max_music);
  if (pf) fclose(pf);
  exit(ret_value);
}

/**
 * @brief Builds a BST that contains every song stored in the radio.
 *
 * Songs are inserted in the order returned by the radio array, which keeps
 * the BST unbalanced (and therefore exercises the recursive routines on a
 * realistic shape).
 *
 * @param radio Pointer to the radio.
 *
 * @return The new tree, or NULL on error.
 */
static BSTree *loadTreeFromRadio(Radio *radio) {
  BSTree *tree = NULL;
  Music *song = NULL;
  int i, n;

  if (!radio) {
    return NULL;
  }

  n = radio_getNumberOfMusic(radio);
  if (n < 0) {
    return NULL;
  }

  tree = tree_init(music_plain_print, music_cmp);
  if (!tree) {
    return NULL;
  }

  for (i = 0; i < n; i++) {
    song = radio_getMusicAt(radio, i);
    if (!song || tree_insert(tree, song) == ERROR) {
      tree_destroy(tree);
      return NULL;
    }
  }

  return tree;
}

/**
 * @brief Builds a synthetic Music object that can be used as a range bound
 * when no real song with the requested id exists in the radio.
 *
 * The BST comparison function (music_cmp) compares songs first by id, so
 * a synthetic Music with just the id set serves as a valid bound *as long
 * as no real song shares that id*. When a real song shares the id, the
 * synthetic bound would be ordered against it by the title field, which
 * is unreliable; in that case the caller should use the real song instead.
 *
 * @param id Song id.
 *
 * @return Pointer to the new Music, or NULL on error.
 */
static Music *make_bound(long id) {
  Music *m = music_init();
  if (!m) {
    return NULL;
  }
  if (music_setId(m, id) == ERROR) {
    music_free(m);
    return NULL;
  }
  return m;
}

int main(int argc, char const *argv[]) {
  FILE *f_in = NULL;
  Radio *radio = NULL;
  BSTree *tree = NULL;
  List *result = NULL;
  Music *min_music = NULL, *max_music = NULL;
  Music *real_min = NULL, *real_max = NULL;
  long min_id, max_id;

  if (argc != MIN_ARGC) {
    fprintf(stderr, "Usage: %s music_file min_id max_id\n", argv[0]);
    return ERROR_MAIN;
  }

  min_id = atol(argv[2]);
  max_id = atol(argv[3]);
  if (min_id < 0 || max_id < 0) {
    fprintf(stderr, "Invalid id range: [%ld, %ld]\n", min_id, max_id);
    return ERROR_MAIN;
  }

  f_in = fopen(argv[1], "r");
  if (!f_in) {
    fprintf(stderr, "ERROR: Could not open input file\n");
    return ERROR_MAIN;
  }

  radio = radio_init();
  if (!radio) {
    mainCleanUp(ERROR_MAIN, tree, radio, min_music, max_music, f_in);
  }

  if (radio_readFromFile(f_in, radio) == ERROR) {
    fprintf(stderr, "ERROR: Could not read the radio\n");
    mainCleanUp(ERROR_MAIN, tree, radio, min_music, max_music, f_in);
  }

  tree = loadTreeFromRadio(radio);
  if (!tree) {
    fprintf(stderr, "ERROR: Could not build the tree\n");
    mainCleanUp(ERROR_MAIN, tree, radio, min_music, max_music, f_in);
  }

  /* Per the practice statement, the range bounds are "the 2 songs whose ids
     are the requested ones": prefer the real Music objects living in the
     radio so music_cmp compares correctly when the boundary id is present.
     If the requested id is not in the radio, fall back to a synthetic
     Music: in that case no real song shares its id, so the id-only
     comparison is enough to position the bound correctly. */
  real_min = radio_getMusicById(radio, min_id);
  real_max = radio_getMusicById(radio, max_id);

  if (!real_min) {
    min_music = make_bound(min_id);
    if (!min_music) {
      fprintf(stderr, "ERROR: Could not create the lower bound\n");
      mainCleanUp(ERROR_MAIN, tree, radio, min_music, max_music, f_in);
    }
  }
  if (!real_max) {
    max_music = make_bound(max_id);
    if (!max_music) {
      fprintf(stderr, "ERROR: Could not create the upper bound\n");
      mainCleanUp(ERROR_MAIN, tree, radio, min_music, max_music, f_in);
    }
  }

  result = tree_rangeSearch(tree,
                            real_min ? (void *)real_min : (void *)min_music,
                            real_max ? (void *)real_max : (void *)max_music);
  if (!result) {
    fprintf(stderr, "ERROR: tree_rangeSearch failed\n");
    mainCleanUp(ERROR_MAIN, tree, radio, min_music, max_music, f_in);
  }

  fprintf(stdout, "Lista de canciones desde id %ld hasta id %ld\n", min_id, max_id);
  if (list_isEmpty(result) == FALSE) {
    list_print(stdout, result, music_plain_print);
    fprintf(stdout, "\n");
  }

  list_free(result);
  mainCleanUp(EXIT_SUCCESS, tree, radio, min_music, max_music, f_in);
  return EXIT_SUCCESS;
}

/*
 * P2: ¿qué características observas en la lista resultante?, ¿a qué se debe?
 *
 * La lista resultante contiene los elementos ordenados de menor a mayor segun
 * la funcion de comparacion del arbol (en nuestro caso, por id ascendente).
 * Esto se debe a que tree_rangeSearch realiza un recorrido in-order
 * (izquierda - nodo - derecha) sobre el BST, y el recorrido in-order de un
 * arbol binario de busqueda visita los nodos en orden creciente segun la
 * funcion de comparacion. Ademas, gracias a la "poda inteligente" (solo se
 * desciende a un subarbol cuando la cota lo permite), se evita visitar las
 * ramas que estan completamente fuera del rango [min, max], lo que hace la
 * operacion mas eficiente que una busqueda lineal.
 */
