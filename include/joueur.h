#ifndef JOUEUR_H
#define JOUEUR_H

#include "piece.h"
#include "echiquier.h"

// Opération sur les joueurs
void initialiseJoueur(Case* Echiquier[8][8], Piece* Joueur[16], int couleur);
void videJoueur(Piece* Joueur[16]);

#endif // JOUEUR_H