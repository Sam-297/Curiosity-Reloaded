#include <stdio.h>
#include <stdlib.h>
#include "meslistes.h"

void somme(sequence_t *pile){
    int v2 = depiler(pile);
    int v1 = depiler(pile);
    ajout_en_tete(pile, v1 + v2);
}

int main(){
    sequence_t *pile = (sequence_t *)malloc(sizeof(sequence_t));
    ajout_en_tete(pile, 1);
    ajout_en_tete(pile, 2);
    ajout_en_tete(pile, 3);
    afficher(pile);
    somme(pile);
    somme(pile);
    afficher(pile);
    return 0;
}