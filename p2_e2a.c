#include <stdio.h>
#include <stdlib.h>

#include "music.h"
#include "p2_utils.h"
#include "radio.h"
#include "stack.h"

#define ERROR_MAIN 1
#define MIN_ARGC 3

/**
 * @brief Runs exercise 2A.
 *
 * The function reads two playlists ordered by duration, loads both playlists
 * into two stacks, merges them into a result stack, and prints the result.
 *
 * @param argc Number of command line arguments.
 * @param argv Command line arguments.
 *
 * @return 0 on success, ERROR_MAIN on failure.
 */
int main(int argc, char **argv) {
    Radio *r1 = NULL, *r2 = NULL;
    Stack *s1 = NULL, *s2 = NULL, *sout = NULL;

    if (argc < MIN_ARGC) {
        fprintf(stderr, "Usage: %s <playlist1.txt> <playlist2.txt>\n", argv[0]);
        return ERROR_MAIN;
    }

    if (load_radio_stack(argv[1], &r1, &s1) == ERROR ||
        load_radio_stack(argv[2], &r2, &s2) == ERROR) {
        fprintf(stderr, "ERROR: Could not read the input playlists\n");
        radio_free(r1);
        radio_free(r2);
        stack_free(s1);
        stack_free(s2);
        return ERROR_MAIN;
    }

    sout = stack_init();
    if (!sout || mergeStacks(s1, s2, sout, music_duration_cmp) == ERROR) {
        fprintf(stderr, "ERROR: Could not merge the playlists\n");
        radio_free(r1);
        radio_free(r2);
        stack_free(s1);
        stack_free(s2);
        stack_free(sout);
        return ERROR_MAIN;
    }

    stack_print(stdout, sout, music_stack_print);
    printf("\n");

    radio_free(r1);
    radio_free(r2);
    stack_free(s1);
    stack_free(s2);
    stack_free(sout);
    return 0;
}
