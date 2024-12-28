#include <stdlib.h> // malloc(), free(), exit()

#include ".././include/joueur.h"

void initialiseJoueur(Case* Echiquier[8][8], Piece* Joueur[16], int couleur) {
    /*
    Initialise les pièces de chacun des joueurs.
    */

    int rangeePromotion, rangeePions;
    switch(couleur) {
        case BLANC :
            rangeePromotion = 0;
            rangeePions = 1;
            break;
        case NOIR :
            rangeePromotion = 7;
            rangeePions = 6;
            break;
    }

    // Roi
    Echiquier[rangeePromotion][4]->piece = creationPiece(ROI, couleur);
    Echiquier[rangeePromotion][4]->piece->x = rangeePromotion;
    Echiquier[rangeePromotion][4]->piece->y = 4;
    Joueur[4] = Echiquier[rangeePromotion][4]->piece;

    // Reine
    Echiquier[rangeePromotion][3]->piece = creationPiece(REINE, couleur);
    Echiquier[rangeePromotion][3]->piece->x = rangeePromotion;
    Echiquier[rangeePromotion][3]->piece->y = 3;
    Joueur[3] = Echiquier[rangeePromotion][3]->piece;

    // Fous
    Echiquier[rangeePromotion][2]->piece = creationPiece(FOU, couleur);
    Echiquier[rangeePromotion][2]->piece->x = rangeePromotion;
    Echiquier[rangeePromotion][2]->piece->y = 2;
    Joueur[2] = Echiquier[rangeePromotion][2]->piece;

    Echiquier[rangeePromotion][5]->piece = creationPiece(FOU, couleur);
    Echiquier[rangeePromotion][5]->piece->x = rangeePromotion;
    Echiquier[rangeePromotion][5]->piece->y = 5;
    Joueur[5] = Echiquier[rangeePromotion][5]->piece;

    // Cavaliers
    Echiquier[rangeePromotion][1]->piece = creationPiece(CAVALIER, couleur);
    Echiquier[rangeePromotion][1]->piece->x = rangeePromotion;
    Echiquier[rangeePromotion][1]->piece->y = 1;
    Joueur[1] = Echiquier[rangeePromotion][1]->piece;

    Echiquier[rangeePromotion][6]->piece = creationPiece(CAVALIER, couleur);
    Echiquier[rangeePromotion][6]->piece->x = rangeePromotion;
    Echiquier[rangeePromotion][6]->piece->y = 6;
    Joueur[6] = Echiquier[rangeePromotion][6]->piece;

    // Tours
    Echiquier[rangeePromotion][0]->piece = creationPiece(TOUR, couleur);
    Echiquier[rangeePromotion][0]->piece->x = rangeePromotion;
    Echiquier[rangeePromotion][0]->piece->y = 0;
    Joueur[0] = Echiquier[rangeePromotion][0]->piece;

    Echiquier[rangeePromotion][7]->piece = creationPiece(TOUR, couleur);
    Echiquier[rangeePromotion][7]->piece->x = rangeePromotion;
    Echiquier[rangeePromotion][7]->piece->y = 7;
    Joueur[7] = Echiquier[rangeePromotion][7]->piece;

    // Pions
    for (int colonne = 0; colonne < 8; colonne++) {
        Echiquier[rangeePions][colonne]->piece = creationPiece(PION, couleur);
        Echiquier[rangeePions][colonne]->piece->x = rangeePions;
        Echiquier[rangeePions][colonne]->piece->y = colonne;
        Joueur[8 + colonne] = Echiquier[rangeePions][colonne]->piece;
    }
}

void videJoueur(Piece* Joueur[16]) {
    /*
    Lièbre la mémoire allouée par chacune des pièces du joueur.
    */

    for (int piece = 0; piece < 16; piece++) { 
        free(Joueur[piece]);
        Joueur[piece] = NULL;
    }
}