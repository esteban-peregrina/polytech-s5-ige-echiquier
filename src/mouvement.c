#include <stdlib.h> // abs()

#include ".././include/mouvement.h"

Piece* mouvement(Case* Echiquier[8][8], Piece* piece, Case* caseCible, bool estSimulation) {
    int xCible = caseCible->x;
    int yCible = caseCible->y;

    int xPrecedent = piece->x;
    int yPrecedent = piece->y;

    // Capture standard
    Piece* pieceCapturee = caseCible->piece;

    // Déplacement standard
    Echiquier[xPrecedent][yPrecedent]->piece = NULL;
    caseCible->piece = piece; // On écrase l'adresse de la pièce présente avec la notre
    piece->x = xCible;
    piece->y = yCible;
    
    // Prise en passant
    int rangeeEnPassant = (piece->couleur == BLANC) ? 4 : 3;
    if ((xPrecedent == rangeeEnPassant) && (piece->role == PION) && (abs(yCible - yPrecedent) == 1) && (!pieceCapturee)) {
        pieceCapturee = Echiquier[xPrecedent][yCible]->piece; // On capture à droite ou à gauche
        if ((pieceCapturee) && (pieceCapturee->vientDeFaireDoublePas)) { 
            Echiquier[xPrecedent][yCible]->piece = NULL;
            if (estSimulation) { piece->vientDePrendreEnPassant = true; } // Sert à rétablir le mouvement correctement
        } else { pieceCapturee = NULL; } // On annule la prise en passant
    }

    if (pieceCapturee) { pieceCapturee->estCapturee = true; }
    
    if (!estSimulation) {
        // Réinitialisation de vientDeFaireDoublePas (donc forcément sur une rangée bien spécifique)
        for (int i = 8; i < 16; i++) {
            if ((piece->couleur == BLANC && Echiquier[3][i-8]->piece && Echiquier[3][i-8]->piece->couleur == BLANC) ||
                (piece->couleur == NOIR && Echiquier[4][i-8]->piece && Echiquier[4][i-8]->piece->couleur == NOIR)) 
            { 
                int rangeeDoublePas = piece->couleur == BLANC ? 3 : 4;
                Echiquier[rangeeDoublePas][i-8]->piece->vientDeFaireDoublePas = false; 
            }
        }

        // Mise à jour des états
        piece->aPrecedemmentBouge = true;

        // Double pas
        if ((piece->role == PION) && (abs(xCible - xPrecedent) == 2)) {
            piece->vientDeFaireDoublePas = true;
        } else {
            piece->vientDeFaireDoublePas = false;
        }
    }
    
    return pieceCapturee;
}