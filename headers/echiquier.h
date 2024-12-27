#ifndef ECHIQUIER_H
#define ECHIQUIER_H

#include <stdbool.h>

#include ".././headers/piece.h"

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

// Déclaration et suppression de cases
Case* creationCase(int x, int y);

// Opérations sur l'echiquier
void initialiseEchiquier(Case* Echiquier[8][8]);
void videEchiquier(Case* Echiquier[8][8]);
void afficheEchiquier(Case* Echiquier[8][8]);

// Jeu
void partieEchec();

#endif // ECHIQUIER_H