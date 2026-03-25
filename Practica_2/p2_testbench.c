#include <stdio.h>

#include "music.h"
#include "p2_utils.h"
#include "radio.h"
#include "stack.h"
#include "types.h"

void test_merge_empty() {
  printf("\n--- Test: Merging Empty Stacks ---\n");
  Stack *s1 = stack_init();
  Stack *s2 = stack_init();
  Stack *sout = stack_init();

  if (mergeStacks(s1, s2, sout, music_duration_cmp) == OK) {
    printf("Result: SUCCESS (Empty merge worked)\n");
  } else {
    printf("Result: FAILURE (Empty merge failed)\n");
  }

  stack_free(s1);
  stack_free(s2);
  stack_free(sout);
}

void test_merge_duration() {
  printf("\n--- Test: Merge by Duration (Descending to Ascending) ---\n");
  Stack *s1 = stack_init();
  Stack *s2 = stack_init();
  Stack *sout = stack_init();

  Music *m1 = music_initFromString(
      "id:\"1\" title:\"A\" artist:\"Art 1\" duration:\"300\"");
  Music *m2 = music_initFromString(
      "id:\"2\" title:\"B\" artist:\"Art 2\" duration:\"200\"");
  Music *m3 = music_initFromString(
      "id:\"3\" title:\"C\" artist:\"Art 3\" duration:\"250\"");
  Music *m4 = music_initFromString(
      "id:\"4\" title:\"D\" artist:\"Art 4\" duration:\"150\"");

  // Order descending for input stacks
  stack_push(s1, m2); // 200
  stack_push(s1, m1); // 300 (top)

  stack_push(s2, m4); // 150
  stack_push(s2, m3); // 250 (top)

  if (mergeStacks(s1, s2, sout, music_duration_cmp) == OK) {
    printf("Merged Stack (should be 150, 200, 250, 300):\n");
    stack_print(stdout, sout, music_stack_print);
    printf("\n");
  } else {
    printf("Result: FAILURE\n");
  }

  stack_free(s1);
  stack_free(s2);
  stack_free(sout);
  music_free(m1);
  music_free(m2);
  music_free(m3);
  music_free(m4);
}

void test_merge_artist() {
  printf("\n--- Test: Merge by Artist (Z-A to A-Z) ---\n");
  Stack *s1 = stack_init();
  Stack *s2 = stack_init();
  Stack *sout = stack_init();

  Music *m1 = music_initFromString(
      "id:\"1\" title:\"A\" artist:\"Zebra\" duration:\"100\"");
  Music *m2 = music_initFromString(
      "id:\"2\" title:\"B\" artist:\"Apple\" duration:\"100\"");
  Music *m3 = music_initFromString(
      "id:\"3\" title:\"C\" artist:\"Yellow\" duration:\"100\"");
  Music *m4 = music_initFromString(
      "id:\"4\" title:\"D\" artist:\"Banana\" duration:\"100\"");

  // Descending alphabetical (Z-A) for top
  stack_push(s1, m2); // Apple
  stack_push(s1, m1); // Zebra (top)

  stack_push(s2, m4); // Banana
  stack_push(s2, m3); // Yellow (top)

  if (mergeStacks(s1, s2, sout, music_artist_cmp) == OK) {
    printf("Merged Stack (should be Apple, Banana, Yellow, Zebra):\n");
    stack_print(stdout, sout, music_stack_print);
    printf("\n");
  } else {
    printf("Result: FAILURE\n");
  }

  stack_free(s1);
  stack_free(s2);
  stack_free(sout);
  music_free(m1);
  music_free(m2);
  music_free(m3);
  music_free(m4);
}

void test_dfs_cases() {
  printf("\n--- Test: DFS Cases ---\n");
  Radio *r = radio_init();

  radio_newMusic(r, "id:\"1\" title:\"S1\" artist:\"A\" duration:\"100\"");
  radio_newMusic(r, "id:\"2\" title:\"S2\" artist:\"A\" duration:\"100\"");
  radio_newMusic(r, "id:\"3\" title:\"S3\" artist:\"A\" duration:\"100\"");

  radio_newRelation(r, 1, 2);
  radio_newRelation(r, 2, 3);

  printf("1. Path 1 -> 3 (Expected: Success):\n");
  if (radio_depthSearch(r, 1, 3) == OK) {
    printf("Result: SUCCESS\n");
  } else {
    printf("Result: FAILURE\n");
  }

  printf("\n2. Non-existent song (Expected: Error):\n");
  if (radio_depthSearch(r, 1, 99) == ERROR) {
    printf("Result: SUCCESS (Properly handled error)\n");
  } else {
    printf("Result: FAILURE\n");
  }

  printf("\n3. No path (1 -> 1 where 1 has no self-cycle and we search for "
         "another node later):\n");
  Radio *r2 = radio_init();
  radio_newMusic(r2, "id:\"1\" title:\"S1\" artist:\"A\" duration:\"100\"");
  radio_newMusic(r2, "id:\"2\" title:\"S2\" artist:\"A\" duration:\"100\"");
  if (radio_depthSearch(r2, 1, 2) == ERROR) {
    printf("Result: SUCCESS (Properly handled no path)\n");
  } else {
    printf("Result: FAILURE\n");
  }

  radio_free(r);
  radio_free(r2);
}

int main() {
  printf("=== PRACTICA 2 TESTBENCH ===\n");

  test_merge_empty();
  test_merge_duration();
  test_merge_artist();
  test_dfs_cases();

  printf("\n=== TESTBENCH FINISHED ===\n");
  return 0;
}
