#ifndef ECHIQUIER_H
#define ECHIQUIER_H

#include <stdbool.h>

#include "piece.h"

typedef enum {
    PIECES,            
    COUPS,
} Menu;

typedef struct Piece Piece; // Pour que le compilateur les reconnaissent

typedef struct Case {
    Piece* piece;
    bool estAtteignableParPiece;
    int estAtteignableParJoueur[2];
    bool estSelectionnee;

    int x;
    int y;
} Case;

// Déclaration de cases
Case* creationCase(int x, int y);

// Opérations sur l'echiquier
void initialiseEchiquier(Case* Echiquier[8][8]);
void videEchiquier(Case* Echiquier[8][8]);

// Jeu
void partieEchec(Case* Echiquier[8][8], Piece *Blancs[16], int* scoreBlancs, Piece *Noirs[16], int* scoreNoirs, int couleurJoueurCourant);
void jeuEchec();

#endif // ECHIQUIER_H