#include    <stdlib.h>
#include    <stdio.h>
#include    <string.h>
#include    "music.h"
#include    "radio.h"
#include    "stack.h"
#include    "p2_utils.h"
#define     ERROR_MAIN  1
#define     MIN_ARGC    2

/**
 * @brief Shows the player menu and returns the selected option.
 *
 * @param history History stack.
 *
 * @return Selected option.
 */
int show_player_menu(Stack *history);

/**
 * @brief Runs exercise 1.
 *
 * The function reads the radio from the input file, stores all songs in a
 * history stack, and allows the user to go back through the played songs.
 *
 * @param argc Number of command line arguments.
 * @param argv Command line arguments.
 *
 * @return 0 on success, ERROR_MAIN on failure.
 */
int main (int argc, char **argv) {

    FILE*   fin = NULL;
    Radio*  new_radio = NULL;
    Stack*  new_stack = NULL;
    Music*  mus = NULL;
    int     i, num_music, option;

    if(!argv || argc < MIN_ARGC){
        printf("ERROR: Introduce the name of the document\n");
        return ERROR_MAIN;
    }

    fin = fopen(argv[1], "r");
    if(!fin){
        printf("ERROR: Don´t read from file\n");
        return ERROR_MAIN;
    }

    new_radio = radio_init();
    new_stack = stack_init();

    if(!new_radio || !new_stack){
        fclose(fin);
        printf("ERROR: Creation of radio or stack\n");
        radio_free(new_radio);
        stack_free(new_stack);
        return ERROR_MAIN;
    }

    if(radio_readFromFile(fin, new_radio) == ERROR){
        fclose(fin);
        printf("ERROR: Don´t read from file\n");
        radio_free(new_radio);
        stack_free(new_stack);
        return ERROR_MAIN;
    }
    fclose(fin);

    if((num_music = radio_getNumberOfMusic(new_radio)) == ERROR_I){
        printf("ERROR: Don´t read number of music on radio\n");
        radio_free(new_radio);
        stack_free(new_stack);
        return ERROR_MAIN;
    }

    for (i = 0; i < num_music; i++){
        mus = radio_getMusicAt(new_radio, i);
        if(!mus || stack_push(new_stack, mus) == ERROR){
            printf("ERROR: Stack push failed\n");
            radio_free(new_radio);
            stack_free(new_stack);
            return ERROR_MAIN;
        }
    }

    do {
        option = show_player_menu(new_stack);

        if(option == 1 && stack_isEmpty(new_stack) == FALSE){
            stack_pop(new_stack);
        }
    } while(option != 2);

    stack_free(new_stack);
    radio_free(new_radio);

    return 0;
}

/**
 * @brief Shows the current song, the history stack and the menu.
 *
 * @param history History stack.
 *
 * @return Selected option.
 */
int show_player_menu(Stack *history) {
    int option;
    Music *m = NULL;

    if (!history) {
        return 2;
    }
	
	/* Obtenemos canción actual del top de la pila */
	m = (Music *)stack_top (history);
    if (m != NULL) {
		music_formatted_print(stdout, m);
    } else {
        printf("\nNo song currently playing.\n");
    }
	
	/* Imprimimos historial (pila) */
	printf("\nRecently Played:\n");
	stack_print(stdout, history, music_stack_print);
	
	/* Mostramos menu y esperamos selección */
    printf("\n1. Back to previous song\n");
    printf("2. Exit\n");
    printf("Choose an option: ");

    if (scanf("%d", &option) != 1) {
        return 2;
    }

    return option;
}
