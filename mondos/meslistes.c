#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "meslistes.h"


/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

bool silent_mode = false;


cellule_t* nouvelleCellule (void)
{
    /* À compléter (utiliser malloc) */
    //printf("\n>>>>>>>>>>> A Faire : liste.c/nouvelleCellule() <<<<<<<<<<<<<<<<\n");
    cellule_t *cel = malloc(sizeof(cellule_t));
    return cel;
}



void detruireCellule (cellule_t* cel)
{
    /* À compléter (utiliser free) */
    //printf("\n>>>>>>>>>>> A Faire : liste.c/detruireCellule() <<<<<<<<<<<<<<<<\n");
    free(cel);
}

void ajout_en_tete(sequence_t *l, int n) {
    /* votre code ici */
    cellule_t *cel = nouvelleCellule();
    cel->command = n;
    cel->suivant = l->tete; 
    l->tete = cel;
    return;
}

int depiler(sequence_t *pile){
    int res;
    cellule_t *tmp = pile->tete;
    pile->tete = tmp->suivant;
    res = tmp->command;
    detruireCellule(tmp);
    return res;
}

void conversion (char *texte, sequence_t *seq)
{
  /* À compléter */
    //printf("\n>>>>>>>>>>> A Faire : liste.c/conversion() <<<<<<<<<<<<<<<<\n");
    int i = 1;
    if (texte[0] != '\0'){
        cellule_t *cel = nouvelleCellule();
        cel->command = texte[0];
        seq->tete = cel;
        cellule_t *tmp = seq->tete;
        while (texte[i] != '\0'){
            cel = nouvelleCellule();
            cel->command = texte[i];
            tmp->suivant = cel;
            tmp = cel;
            i++;
        }
        tmp->suivant = NULL;
    }
}

void afficher (sequence_t* seq)
{
    assert (seq); /* Le pointeur doit être valide */
    /* À compléter */
    //printf("\n>>>>>>>>>>> A Faire : liste.c/afficher() <<<<<<<<<<<<<<<<\n");
    cellule_t *c;
    c = seq->tete;
    while (c!=NULL) {
        printf(" %d",c->command);
        c = c->suivant;
    }
    printf("\n");
}

