#include <stdio.h>
#include <stdlib.h>

#include "bstree.h"
#include "music.h"
#include "radio.h"
#include "types.h"

static void mainCleanUp(int ret_value, BSTree *tree, Radio *radio, FILE *pf) {
  tree_destroy(tree);
  radio_free(radio);
  if (pf) {
    fclose(pf);
  }
  exit(ret_value);
}

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

int main(int argc, char const *argv[]) {
  FILE *f_in = NULL;
  BSTree *tree = NULL;
  Radio *radio = NULL;
  int min_duration, count;

  if (argc != 3) {
    fprintf(stdout, "Usage: %s music_file min_duration\n", argv[0]);
    return EXIT_FAILURE;
  }

  min_duration = atoi(argv[2]);
  if (min_duration < 0) {
    fprintf(stdout, "Invalid min_duration: %s\n", argv[2]);
    return EXIT_FAILURE;
  }

  f_in = fopen(argv[1], "r");
  if (!f_in) {
    return EXIT_FAILURE;
  }

  radio = radio_init();
  if (!radio) {
    mainCleanUp(EXIT_FAILURE, tree, radio, f_in);
  }

  if (radio_readFromFile(f_in, radio) == ERROR) {
    fprintf(stdout, "Not file or File format incorrect\n");
    mainCleanUp(EXIT_FAILURE, tree, radio, f_in);
  }

  tree = loadTreeFromRadio(radio);
  if (!tree) {
    mainCleanUp(EXIT_FAILURE, tree, radio, f_in);
  }

  count = tree_countLongSongs(tree, min_duration);
  if (count == ERROR_I) {
    mainCleanUp(EXIT_FAILURE, tree, radio, f_in);
  }

  fprintf(stdout, "Número de canciones con duracion mayor a %d segundos: %d\n",
          min_duration, count);

  mainCleanUp(EXIT_SUCCESS, tree, radio, f_in);
  return EXIT_SUCCESS;
}

/*
P3: No. The tree is ordered by the comparison function of Music, which sorts songs by id, not by duration.
Since duration is unrelated to tree order,
a node does not let us discard an entire left or right subtree just from its duration,
so the traversal must visit all nodes in the worst case.
*/
