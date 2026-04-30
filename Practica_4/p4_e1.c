#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bstree.h"
#include "radio.h"
#include "types.h"

/* START Private methods */
int mainCleanUp (int ret_value, Radio *r, FILE *pf) {
  radio_free(r);   
  fclose(pf);
  exit(ret_value);
}

BSTree *loadUnbalancedTree(Music **data, int n) {
  BSTree *t;
  Music *m;
  int i;

  if (!data || (n <= 0)) {
    return NULL;
  }

  if (!(t = tree_init(music_plain_print, music_cmp))) {
    return NULL;
  }

  for (i = 0; i < n; i++) {
    m = data[i];
    if (tree_insert(t, m) == ERROR) {
      fprintf(stdout, "Music ");
      music_plain_print(stdout, m);
      fprintf(stdout, " not inserted!\n");
    }
  }

  return t;
}

int qsort_fun(const void *e1, const void *e2){
  Music **pm1, **pm2;

  pm1 = (Music **) e1;
  pm2 = (Music **) e2;

  return music_cmp(*pm1, *pm2);
}
/* END Private methods */


int main(int argc, char const *argv[]) {
	FILE *f_in = NULL, *f_out = NULL;
	BSTree *t = NULL;
	Music **songs=NULL, *m;
	const char *mode;
	int n, index=0;
	long	music_id;
	clock_t elapsed;
	Radio *r = NULL;

	if (argc != 4) {
		printf("Usage: %s music_file music_id mode[normal|sorted]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	mode = argv[3];
	if (strcmp(mode, "normal") && strcmp(mode, "sorted")) {
		printf("Incorrect mode: %s\n", mode);
		exit(EXIT_FAILURE);
	}

    f_in = fopen(argv[1], "r");
    if (!f_in) {
      return (EXIT_FAILURE);
    }
	f_out = stdout;

    r = radio_init();
    if (!r) mainCleanUp (EXIT_FAILURE, r, f_in);
    
    /* Reads the file */
    if  (radio_readFromFile(f_in, r) == ERROR) {
      fprintf(stdout, "Not file or File format incorrect\n");
      mainCleanUp (EXIT_FAILURE, r, f_in);
    }
	
	music_id = atoi(argv[2]);
	n = radio_getNumberOfMusic(r);
	if (n <= 0) {
		mainCleanUp(EXIT_FAILURE, r, f_in);
	}

	songs = malloc((size_t)n * sizeof(Music *));
	if (!songs) {
		mainCleanUp(EXIT_FAILURE, r, f_in);
	}

	for (index = 0; index < n; index++) {
		songs[index] = radio_getMusicAt(r, index);
	}

	m = radio_getMusicById(r, music_id);
	if (m == NULL) {
		printf("Error when initialising music with id: %ld\n", music_id);
		free(songs);
		mainCleanUp (EXIT_FAILURE, r, f_in);
	}

	if (!strcmp(mode, "normal")) {
		fprintf(f_out, "Mode: normal\n");
		elapsed = clock();
		t = loadUnbalancedTree(songs, n);
		elapsed = clock() - elapsed;
	}
	else {
		qsort(songs, n, sizeof(Music *), qsort_fun);
		fprintf(f_out, "Mode: sorted\n");
		elapsed = clock();
		t = loadUnbalancedTree(songs, n);
		elapsed = clock() - elapsed;
	}

  if (!t) {
    mainCleanUp (EXIT_FAILURE, r, f_in);
  }

  fprintf(f_out, "Tree building time: %ld ticks (%f seconds)\n", (long)elapsed, ((float) elapsed) / CLOCKS_PER_SEC);
  fprintf(f_out, "Tree size: %zu\nTree depth: %d\n", tree_size(t), tree_depth(t));

  fprintf(f_out, "Min element in tree: ");
  elapsed = clock();
  music_plain_print(f_out, tree_find_min(t));
  elapsed = clock() - elapsed;
  fprintf(f_out, " - %ld ticks (%f seconds)\n", (long)elapsed, ((float) elapsed) / CLOCKS_PER_SEC);

  fprintf(f_out, "Max element in tree: ");
  elapsed = clock();
  music_plain_print(f_out, tree_find_max(t));
  elapsed = clock() - elapsed;
  fprintf(f_out, " - %ld ticks (%f seconds)\n", (long)elapsed, ((float) elapsed) / CLOCKS_PER_SEC);

  elapsed = clock();
  if (tree_contains(t, m) == TRUE) {
    fprintf(f_out, "Element found");
    elapsed = clock() - elapsed;
    fprintf(f_out, " - %ld ticks (%f seconds)\n", (long)elapsed, ((float) elapsed) / CLOCKS_PER_SEC);

  } else {
    fprintf(f_out, "Element NOT found");
    elapsed = clock() - elapsed;
    fprintf(f_out, " - %ld ticks (%f seconds)\n", (long)elapsed, ((float) elapsed) / CLOCKS_PER_SEC);
  }
  
  tree_destroy(t);
  free(songs);
  mainCleanUp (EXIT_SUCCESS, r, f_in);
}
/*
P1: In normal mode, the songs are inserted in the same order as in the input file.
*In sorted mode, the songs are first ordered with qsort and then inserted in that order.
*Since the tree is a regular BST and it is not self-balancing,
*inserting already ordered data can create a very unbalanced tree, almost like a linked list.
*This increases the tree depth, and min, max, contains and insert depend on that depth.
*Therefore the property that explains the differences in time is the depth or height of the tree.
*/
