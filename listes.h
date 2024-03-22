#ifndef LISTES_H
#define LISTES_H

#include <stdbool.h>

/*
 * Pour réaliser des tests de performance, désactiver tous les 
 * affichages.
 * Pour cela, le plus simple est de compiler avec la macro 'SILENT' définie, en 
 * faisant 'make silent'
 */

#ifdef SILENT

/* Desactive tous les affichages */
#define printf(fmt, ...) (0)
#define eprintf(fmt, ...) (0)
#define putchar(c) (0)
/* Desactive les 'Appuyer sur entrée pour continuer' */
#define getchar() ('\n')

#else

#define eprintf(...) fprintf (stderr, __VA_ARGS__)

#endif

extern bool silent_mode;

struct cellule {
    char command;
    /* vous pouvez rajouter d'autres champs ici */
    int num; // ce variable sera utilise pour stocker les entier de calculs dedans
    struct cellule *tete_liste; // pour stocker les groupes de commandes
    struct cellule *queue_liste;
    struct cellule *suivant;
};
typedef struct cellule cellule_t;

struct sequence {
    cellule_t *tete;
};
typedef struct sequence sequence_t;

sequence_t *new_sequence(void);

cellule_t* nouvelleCellule (void);

void detruireCellule (cellule_t*);

void ajout_en_tete(sequence_t *pile, int val); // pour ajouter un entier

void ajout_en_tete_commande(cellule_t *l, char com); // pour ajouter une commande

void ajout_dans_routine(sequence_t *routine, sequence_t *VF); // pour ajouter les commandes du VF dans la routine

int depiler(sequence_t *pile);

int depiler_int(sequence_t *pile);

void conversion (char *texte, sequence_t *seq);

void afficher (sequence_t* seq);

void afficher_pile_entiers(sequence_t *seq);

void detruire_seq(sequence_t *seq);


#endif
