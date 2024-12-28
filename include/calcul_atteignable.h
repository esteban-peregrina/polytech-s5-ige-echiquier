#ifndef CALCUL_ATTEIGNABLE_H
#define CALCUL_ATTEIGNABLE_H

#include "piece.h"
#include "echiquier.h"

void calculAtteignablePion(Case* Echiquier[8][8], Piece* self);
void calculAtteignableCavalier(Case* Echiquier[8][8], Piece* self);
void calculAtteignableTour(Case* Echiquier[8][8], Piece* self);
void calculAtteignableFou(Case* Echiquier[8][8], Piece* self);
void calculAtteignableReine(Case* Echiquier[8][8], Piece* self);
void calculAtteignableRoi(Case* Echiquier[8][8], Piece* self);

#endif // CALCUL_ATTEIGNABLE_H