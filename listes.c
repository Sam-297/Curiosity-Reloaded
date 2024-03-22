#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"


/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

bool silent_mode = false;

sequence_t *new_sequence(void){
    sequence_t *seq = (sequence_t *)malloc(sizeof(sequence_t));
    seq->tete = NULL;
    return seq;
}

cellule_t* nouvelleCellule (void)
{
    cellule_t *c = malloc(sizeof(cellule_t));
    c->num=-1;
    c->command='\0';
    c->tete_liste=NULL;
    c->queue_liste=NULL;
    c->suivant=NULL;
    return c;
}



void detruireCellule (cellule_t* cel)
{
    free(cel);
}

void ajout_en_tete(sequence_t *l, int val) { // pour les entiers
    cellule_t *cel = nouvelleCellule();
    cel->num = val;
    cel->suivant = l->tete; 
    l->tete = cel;
    return;
}

void ajout_en_tete_commande(cellule_t *l, char com) { // pour les commandes
    cellule_t *cel = nouvelleCellule();
    cel->command = com;
    cel->suivant = l->tete_liste; 
    l->tete_liste = cel;
    return;
}

void ajout_dans_routine(sequence_t *routine, sequence_t *VF){
    cellule_t *cel = VF->tete;
    while (cel != NULL) {
        ajout_en_tete(routine, cel->command);
        routine->tete = cel;
        cel = cel->suivant;
    }
    return;
}

int depiler(sequence_t *pile){
    if (pile->tete == NULL){
        if (! silent_mode) {
            printf("La pile est vide\n");
        }
        return -1;
    }
    int res;
    cellule_t *tmp = pile->tete;
    pile->tete = tmp->suivant;
    res = tmp->command;
    detruireCellule(tmp);
    return res;
}

int depiler_int(sequence_t *pile){
    if (pile->tete == NULL){
        if (! silent_mode) {
            printf("La pile est vide\n");
        }
        return -1;
    }
    int res;
    cellule_t *tmp = pile->tete;
    pile->tete = tmp->suivant;
    res = tmp->num;
    detruireCellule(tmp);
    return res;
}

void conversion(char *texte, sequence_t *seq)
{
    int i = 0;
    seq->tete = NULL; // Initialisez le pointeur de tête à NULL pour gérer le cas où la séquence est vide
    cellule_t *cel = NULL;
    cellule_t *prevCel = NULL;

    while (texte[i] != '\0')
    {
        char c = texte[i++];

        if (c != ' ' && c != '\n' && c != '\0')
        {
            if (cel == NULL)
            {
                // Créez une nouvelle cellule seulement si nécessaire
                cel = nouvelleCellule();
                if (seq->tete == NULL)
                {
                    seq->tete = cel; // Mettez à jour le pointeur de tête si la séquence était vide
                }
                if (prevCel != NULL)
                {
                    prevCel->suivant = cel; // Mettez à jour le lien précédent vers la nouvelle cellule
                }
            }
            cel->command = c;
            cel->suivant = NULL;
            prevCel = cel;      // Mettez à jour le pointeur précédent pour la prochaine itération
            cel = cel->suivant; // N'avancez le pointeur que si une nouvelle cellule est créée
        }
    }
}

void afficher (sequence_t* seq)
{   
    if (! silent_mode) {
        assert (seq); /* Le pointeur doit être valide */
        cellule_t *c;
        c = seq->tete;
        while (c!=NULL) {
            printf(" %c",c->command);
            c = c->suivant;
        }
        printf("\n");
    }
}

void afficher_pile_entiers(sequence_t *seq)
{
    if (! silent_mode) {
        
        assert(seq); // Le pointeur doit être valide

        cellule_t *c;
        c = seq->tete;

        while (c != NULL)
        {
            printf(" %d", c->num); // Assuming num stores integer values
            c = c->suivant;
        }

        printf("\n");
    }
}

void detruire_seq(sequence_t *seq)
{
    while (seq->tete != NULL){
        depiler(seq);
    }
    free(seq);
}