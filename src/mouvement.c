#include <stdlib.h> // abs()

#include ".././include/mouvement.h"

Piece* mouvement(Case* Echiquier[8][8], Piece* piece, int xCible, int yCible, bool estSimulation) {
    int xPrecedent = piece->x;
    int yPrecedent = piece->y;
    Piece* pieceCapturee = NULL;
    
    // Sauvegarde pièce capturée éventuelle
    pieceCapturee = Echiquier[xCible][yCible]->piece;
    
    // Déplacement de base
    Echiquier[xPrecedent][yPrecedent]->piece = NULL;
    Echiquier[xCible][yCible]->piece = piece;
    piece->x = xCible;
    piece->y = yCible;
    
    // Prise en passant
    if (piece->role == PION && abs(yCible - yPrecedent) == 1 && !pieceCapturee) {
        pieceCapturee = Echiquier[xPrecedent][yCible]->piece;
        if (pieceCapturee) {
            if (!estSimulation) pieceCapturee->estCapturee = true;
            Echiquier[xPrecedent][yCible]->piece = NULL;
        }
    }
    
    // Roque
    if (piece->role == ROI && abs(yCible - yPrecedent) == 2) {
        if (yCible > yPrecedent) { // Petit roque
            Piece* tour = Echiquier[xPrecedent][7]->piece;
            Echiquier[xPrecedent][7]->piece = NULL;
            Echiquier[xPrecedent][5]->piece = tour;
            tour->y = 5;
        } else { // Grand roque
            Piece* tour = Echiquier[xPrecedent][0]->piece;
            Echiquier[xPrecedent][0]->piece = NULL;
            Echiquier[xPrecedent][3]->piece = tour;
            tour->y = 3;
        }
    }
    
    if (!estSimulation) {
        // Mise à jour des états
        if (pieceCapturee) { pieceCapturee->estCapturee = true; }
        piece->aPrecedemmentBouge = true;
        if (piece->role == PION && abs(xCible - xPrecedent) == 2) {
            piece->vientDeFaireDoublePas = true;
        }
    }
    
    return pieceCapturee;
}