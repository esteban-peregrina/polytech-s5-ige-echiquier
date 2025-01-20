#ifndef ECHIQUIER_H
#define ECHIQUIER_H

#include <stdbool.h>

#include "piece.h"

#define PVP 0
#define PVE 1
#define EVE 2

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
void jeuEchec();
void partieEchec(Case* Echiquier[8][8], Piece *Blancs[16], int* scoreBlancs, Piece *Noirs[16], int* scoreNoirs, int type);
void tourDuJoueur(Case* Echiquier[8][8], Piece *Blancs[16], int* scoreBlancs, Piece *Noirs[16], int* scoreNoirs, int type);
void tourDeLaMachine(Case* Echiquier[8][8], Piece *Blancs[16], int* scoreBlancs, Piece *Noirs[16], int* scoreNoirs, int type);


#endif // ECHIQUIER_H