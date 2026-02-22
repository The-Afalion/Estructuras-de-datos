#include <stdio.h>
#include "radio.h"

#define ID_orig 222
#define ID_dest 111

int main() {
    Radio    *r = NULL;
    long*    reco_music = NULL;
    int      num_relations_dest = 0;

    r = radio_init();
    if (!r) {
        printf("ERROR: Error initializing radio.\n");
        return 1;
    }
    //Inserting Musics in Radio    
    printf("Inserting Paint It, Black... result...: %d", (int)radio_newMusic(r, "id:\"111\" title:\"Paint It, Black\" artist:\"The Rolling Stones\" duration:\"202\""));
    printf("\n");
    
    printf("Inserting Every Breath You Take... result...: %d",(int)radio_newMusic(r, "id:\"222\" title:\"Every Breath You Take\" artist:\"The Police\" duration:\"253\""));
    printf("\n\n");

    //Inserting relations betwen musics in Radio    
    if(radio_newRelation(r, ID_orig, ID_dest) == ERROR){
        printf("ERROR: Can't Insert radio recomendation\n");
    }else{
        printf("inserting radio recomendation: %d --> %d\n\n", ID_orig, ID_dest);
    }




    printf("%d --> %d? ", ID_dest, ID_orig);
    if(radio_relationExists(r, ID_dest, ID_orig) == TRUE){
        printf("Yes\n");
    }else{
        printf("No\n");
    }
    printf("%d --> %d? ", ID_orig, ID_dest);
    if(radio_relationExists(r, ID_orig, ID_dest) == TRUE){
        printf("Yes\n\n");
    }else{
        printf("No\n");
    }
    num_relations_dest = radio_getNumberOfRelationsFromId(r, ID_orig);
    printf("Number of Connections from %d: %d\n", ID_dest, radio_getNumberOfRelationsFromId(r, ID_dest));
    printf("Number of Connections from %d: %d\n\n", ID_orig,num_relations_dest);



    reco_music = radio_getRelationsFromId(r, ID_orig);
    if(reco_music == NULL || num_relations_dest < 0){
        printf("ERROR: Can't created array with relations of %d", ID_dest);
    }
    printf("Radio recomendations from Paint It, Black: ");
    for (int i = 0; i < num_relations_dest; i++)
        printf("%ld ", reco_music[i]);
    printf("\n\n");



    printf("All radio recomendations: \n");
    radio_print(stdout, r);

    printf("\n\n");

    radio_free(r);
    free(reco_music);

    return 0;
}
