#ifndef MOUVEMENT_H
#define MOUVEMENT_H

#include "piece.h"
#include "echiquier.h"

// Retourne la pièce capturée si il y en a une
Piece* mouvement(Case* Echiquier[8][8], Piece* piece, Case* caseCible, bool estSimulation, int* score);
void mouvementIA(Case* Echiquier[8][8], Piece** joueurIA, int* scoreIA);

#endif