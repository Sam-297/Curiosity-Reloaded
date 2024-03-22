#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"
#include "curiosity.h"

void stop(void)
{
    char enter = '\0';
    if (! silent_mode) {
        printf("Appuyer sur entrée pour continuer...\n");
    }
    while (enter != '\r' && enter != '\n')
    {
        enter = getchar();
    }
}

int verifier_si_entier(int com)
{
    if (com >= '0' && com <= '9')
    {
        return true;
    }
    return false;
}

void somme(sequence_t *pile)
{
    int v2 = depiler_int(pile);
    int v1 = depiler_int(pile);
    ajout_en_tete(pile, v1 + v2);
}

void difference(sequence_t *pile)
{
    int v2 = depiler_int(pile);
    int v1 = depiler_int(pile);
    ajout_en_tete(pile, v1 - v2);
}

void multiplication(sequence_t *pile)
{
    int v2 = depiler_int(pile);
    int v1 = depiler_int(pile);
    ajout_en_tete(pile, v1 * v2);
}

int digits_to_num(char c)
{
    return (int)(c - '0');
}

void gerer_accolade(sequence_t *pile, cellule_t **tmp){ //O(n)
    (*tmp) = (*tmp)->suivant; // after {
    char commande = (*tmp)->command;

    cellule_t *c = nouvelleCellule(); // empiler
    c->suivant = pile->tete;
    pile->tete = c;

    c->tete_liste = (*tmp);

    int nb_groupe = 0;
    if (commande != '}')
    {
        while ((*tmp)->suivant->command != '}' || nb_groupe != 0)
        {
            (*tmp) = (*tmp)->suivant;
            if ((*tmp)->command == '{')
            {
                nb_groupe++;
            }
            if ((*tmp)->command == '}')
            {
                nb_groupe--;
            }
        }
        c->queue_liste = (*tmp);
        (*tmp) = (*tmp)->suivant;
    }
    else
    {
        c->tete_liste = NULL;
        c->queue_liste = NULL;
    }
    c->num = -1;
}

void exec_conditionnelle_bloc(sequence_t *pile, cellule_t *tmp)
{
    int b = pile->tete->suivant->suivant->num;

    if (b == 0)
    {
        if (pile->tete->tete_liste != NULL)
        {
            pile->tete->queue_liste->suivant = tmp->suivant;
            tmp->suivant = pile->tete->tete_liste;
        }
    }
    else
    {
        if (pile->tete->suivant->tete_liste != NULL)
        {
            pile->tete->suivant->queue_liste->suivant = tmp->suivant;
            tmp->suivant = pile->tete->suivant->tete_liste;
        }
    }
    pile->tete = pile->tete->suivant->suivant->suivant;
}

void inverser(sequence_t *pile) { //O(n)
    sequence_t renversee;
    renversee.tete = NULL;

    cellule_t *courant = pile->tete;
    cellule_t *temporaire;

    while (courant != NULL) {
        temporaire = courant->suivant;
        courant->suivant = renversee.tete;
        renversee.tete = courant;
        courant = temporaire;
    }

    pile->tete = renversee.tete;
}

void echanger(sequence_t *pile)
{
    cellule_t *tete_tmp = pile->tete->tete_liste;
    cellule_t *queue_tmp = pile->tete->queue_liste;
    int num_tmp = pile->tete->num;
    char command_tmp = pile->tete->command;

    pile->tete->command = pile->tete->suivant->command;
    pile->tete->num = pile->tete->suivant->num;
    pile->tete->tete_liste = pile->tete->suivant->tete_liste;
    pile->tete->queue_liste = pile->tete->suivant->queue_liste;

    pile->tete->suivant->num = num_tmp;
    pile->tete->suivant->command = command_tmp;
    pile->tete->suivant->tete_liste = tete_tmp;
    pile->tete->suivant->queue_liste = queue_tmp;
}

void depile_et_ajoute_routine(sequence_t *pile, cellule_t *tmp)
{
    if (pile->tete->tete_liste != NULL)
    {
        pile->tete->queue_liste->suivant = tmp->suivant;
        tmp->suivant = pile->tete->tete_liste;
        
    }
    pile->tete = pile->tete->suivant;
}

void cloner(sequence_t *pile){ //O(n)
    cellule_t *c = nouvelleCellule();
    cellule_t *tmp = pile->tete->tete_liste;
    c->command = pile->tete->command;
    c->num = pile->tete->num;
    if (pile->tete->tete_liste != NULL){
        cellule_t *new_c = nouvelleCellule();
        c->tete_liste = new_c;

        while (tmp != pile->tete->queue_liste->suivant){
            new_c->command = tmp->command;
            new_c->num = tmp->num;
            
            if (tmp->suivant == pile->tete->queue_liste->suivant){
                c->queue_liste = new_c;
            }
            else {
                new_c->suivant = nouvelleCellule();
                new_c = new_c->suivant;
            }
            tmp = tmp->suivant;
        }
    }
    c->suivant = pile->tete;
    pile->tete = c;
}

void boucle(sequence_t *pile, cellule_t **tmp)
{
    cellule_t *cmd = pile->tete->suivant;

    if (pile->tete->num > 0)
    {
        cellule_t *b = nouvelleCellule();
        b->command = (*tmp)->command;
        b->suivant = (*tmp)->suivant;

        (pile->tete->num)--;
        cmd->queue_liste->suivant = b;
        (*tmp)->suivant = cmd->tete_liste;
    }
    else
    {
        pile->tete = pile->tete->suivant->suivant;
        //(*tmp) = (*tmp)->suivant; //FIXME: without: segfault, with: boucle infini?
    }
}

void ignore_sommet_pile(sequence_t *pile)
{
    cellule_t *tmp = pile->tete;
    pile->tete = pile->tete->suivant;
    detruireCellule(tmp);
}

void rotation(sequence_t *pile) //O(n^2)
{
    int x = depiler_int(pile);
    int n = depiler_int(pile);
    cellule_t *cel = pile->tete;
    cellule_t *tmp;

    if (n > 1 && x > 0)
    {
        for (int i = 0; i < x; i++)
        {
            cel = pile->tete;
            for (int j = 0; j < n - 2; j++) // arriver à l'avant dernier
            { 
                cel = cel->suivant;
            }
            tmp = cel->suivant->suivant; // après les x élements

            cel->suivant->suivant = pile->tete;

            pile->tete = cel->suivant;

            cel->suivant = tmp;
        }            
    }
}

int interprete(sequence_t *seq, bool debug)
{
    char commande;
    if (! silent_mode) {
        printf("Programme:");
        afficher(seq);
        printf("\n");
    }
    if (debug)
        stop();

    int ret = 0;

    cellule_t *tmp = seq->tete;

    sequence_t *pile = new_sequence();
    while (tmp != NULL)
    {
        commande = tmp->command;
        switch (commande)
        {
        case 'A':
            ret = avance();
            if (ret == VICTOIRE)
                return VICTOIRE;
            if (ret == RATE)
                return RATE;
            break;
        case 'D':
            droite();
            break;
        case 'G':
            gauche();
            break;
        case '0' ... '9':
            ajout_en_tete(pile, digits_to_num(commande));
            break;
        case '+':
            somme(pile);
            break;
        case '-':
            difference(pile);
            break;
        case '*':
            multiplication(pile);
            break;
        case 'P':
            pose(depiler_int(pile));
            break;
        case 'M':
            ajout_en_tete(pile, mesure(depiler_int(pile)));
            break;
        case '{':
            gerer_accolade(pile, &tmp);
            break;
        case '?':
            exec_conditionnelle_bloc(pile, tmp);
            break;
        case 'Z':
            inverser(pile);
            break;
        case 'X':
            echanger(pile);
            break;
        case '!':
            depile_et_ajoute_routine(pile, tmp);
            break;
        case 'C':
            cloner(pile);
            break;
        case 'B':
            boucle(pile, &tmp);
            break;
        case 'I':
            ignore_sommet_pile(pile);
            break;
        case 'R':
            rotation(pile);
            break;
        default:
            if (! silent_mode) {
                printf("Caractère inconnu: '%c'\n", commande);
            }
        }
        if (! silent_mode) {
            printf("######################################\n");
            printf("Pile:");
            afficher_pile_entiers(pile);
        }
        
        tmp = tmp->suivant;
        seq->tete = tmp->suivant;

        if (! silent_mode) {
            afficherCarte();
            printf("Programme:");
            afficher(seq);
            printf("\n");
        }
        if (debug)
            stop();
    }
    return CIBLERATEE;
}
