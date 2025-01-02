#include <stdlib.h> // abs()

#include ".././include/mouvement.h"

Piece* mouvement(Case* Echiquier[8][8], Piece* piece, Case* caseCible, bool estSimulation, int* score) {
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
    
    if (!pieceCapturee) {
        // Prise en passant
        int rangeeEnPassant = (piece->couleur == BLANC) ? 4 : 3;
        if ((xPrecedent == rangeeEnPassant) && (piece->role == PION) && (abs(yCible - yPrecedent) == 1)) {
            pieceCapturee = Echiquier[xPrecedent][yCible]->piece; // On capture à droite ou à gauche
            if ((pieceCapturee) && (pieceCapturee->vientDeFaireDoublePas)) { 
                Echiquier[xPrecedent][yCible]->piece = NULL;
                pieceCapturee->estCapturee = true;
                if (estSimulation) { piece->vientDePrendreEnPassant = true; } // Sert à rétablir le mouvement correctement
            } else { pieceCapturee = NULL; } // On annule la prise en passant
        }
               
        // Roque
        else if ((piece->role == ROI) && (yPrecedent == 4)) { //? Le chemin retour de la simulation après un roque ne doit pas être pris en compte ! On peut aussi utiliser aPrecedemmentBouge
            if (yCible == 6) {
                // Petit roque
                Piece* tourDroite = Echiquier[xCible][7]->piece;
                tourDroite->y = 5;
                Echiquier[xCible][5]->piece = tourDroite;
                Echiquier[xCible][7]->piece = NULL;
                if (estSimulation) { piece->vientDeRoquer = true; } // Sert à rétablir le mouvement correctement
                else { tourDroite->aPrecedemmentBouge = true; }
            } else if (yCible == 2) {
                // Grand roque
                Piece* tourGauche = Echiquier[xCible][0]->piece;
                tourGauche->y = 3;
                Echiquier[xCible][3]->piece = tourGauche;
                Echiquier[xCible][0]->piece = NULL;
                if (estSimulation) { piece->vientDeRoquer = true; } // Sert à rétablir le mouvement correctement
                else { tourGauche->aPrecedemmentBouge = true; }
            }
        }
        
    } else { pieceCapturee->estCapturee = true; }
    
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

        if (pieceCapturee) { (*score)+= pieceCapturee->role; }
    }
    
    return pieceCapturee;
}