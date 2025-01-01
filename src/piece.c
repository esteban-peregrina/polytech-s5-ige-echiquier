#include <stdlib.h> // malloc(), free(), exit(), abs()

#include ".././include/calcul_atteignable.h" // calculAtteignablePion(), calculAtteignableCavalier(), calculAtteignableTour(), calculAtteignableFou(), calculAtteignableReine(), calculAtteignableRoi()
#include ".././include/mouvement.h" // mouvement()

#include ".././include/affichage.h" // affichage() - DEBUG

#include ".././include/piece.h"

Piece* creationPiece(Role role, int couleur) {
    /*
    Renvoie l'adresse de la Piece créée, initialisée avec le role et la couleur specifié.

    Remarque : La structure n'ayant pas de membres alloués dynamiquement, un simple appel de free() suffit à la détruire.
    */

    Piece* pieceCree = NULL;
    pieceCree = malloc(sizeof(Piece));
    if (pieceCree == NULL) { exit(EXIT_FAILURE); }

    pieceCree->role = role;

    if (role == ROI) {
        pieceCree->forme = "♚";
        pieceCree->calculAtteignable = calculAtteignableRoi;
    } else if (role == REINE) {
        pieceCree->forme = "♛";
        pieceCree->calculAtteignable = calculAtteignableReine;
    } else if (role == FOU) {
        pieceCree->forme = "♝";
        pieceCree->calculAtteignable = calculAtteignableFou;
    } else if (role == CAVALIER) {
        pieceCree->forme = "♞";
        pieceCree->calculAtteignable = calculAtteignableCavalier;
    } else if (role == TOUR) {
        pieceCree->forme = "♜";
        pieceCree->calculAtteignable = calculAtteignableTour;
    } else if (role == PION) {
        pieceCree->forme = "♟";
        pieceCree->calculAtteignable = calculAtteignablePion;
    }
    pieceCree->couleur = couleur;
    pieceCree->x = 0;
    pieceCree->y = 0;

    for (int i = 0; i < COUVERTUREMAX; i++) { pieceCree->casesAtteignables[i] = NULL; }

    pieceCree->estSelectionnee = false;

    pieceCree->estCapturee = false;
    pieceCree->estBloquee = false;
    pieceCree->aPrecedemmentBouge = false; 

    pieceCree->vientDeFaireDoublePas = false; 
    pieceCree->vientDePrendreEnPassant = false;
    
    pieceCree->vientDeRoquer = false;

    pieceCree->longueurCasesAtteignables = 0;

    return pieceCree;
}

void insertionCasesAtteignables(Piece* pieceCourante, Case* caseAtteignable) {
    /* 
    Insère caseAtteignable dans le tableau de casesAtteignables de pieceCourante, après le dernier élément non-nul.
    */

    // On ajoute la case atteignable à la fin du tableau
    int i = 0;
    while ( (i < COUVERTUREMAX) && (pieceCourante->casesAtteignables[i] != NULL) ) { i++; } // On cherche la fin du tableau
    if (i >= COUVERTUREMAX) { exit(69); } // Il y davantage de cases atteignables qu'il y a de cases dans le plateau
    pieceCourante->casesAtteignables[i] = caseAtteignable;
    
    // On met à jour les statuts de la pièce
    pieceCourante->longueurCasesAtteignables++;

    // On met à jour l'état de couverture de la case atteignable
    caseAtteignable->estAtteignableParJoueur[pieceCourante->couleur]++;
}

 void suppressionCasesAtteignables(Piece* pieceCourante, Case* caseSupprimable) {
    /*
    Supprime caseSupprimable dans le tableau de casesAtteignables de pieceCourante.
    */

    // On supprime la case
    int i = 0;
    while ( (i < COUVERTUREMAX) && (pieceCourante->casesAtteignables[i] != caseSupprimable) ) { i++; } // On cherche la caseSupprimable dans le tableau
    if (i >= COUVERTUREMAX) { exit(68); } // La pièce n'est pas dans le tableau
    pieceCourante->casesAtteignables[i] = NULL;

    // On décale les cases suivantes pour combler le trou
    for (int j = i; j < pieceCourante->longueurCasesAtteignables - 1; j++) { 
        pieceCourante->casesAtteignables[j] = pieceCourante->casesAtteignables[j + 1];
    }
    pieceCourante->casesAtteignables[pieceCourante->longueurCasesAtteignables - 1] = NULL;

    // On met à jour les statuts de la pièce
    pieceCourante->longueurCasesAtteignables--;
    if (pieceCourante->longueurCasesAtteignables == 0) { pieceCourante->estBloquee = true; } // La pièce n'a plus de cases atteignables

    // On met à jour l'état de couverture de la case supprimée
    caseSupprimable->estAtteignableParJoueur[pieceCourante->couleur]--;
 }

void actualiseCasesAtteignablesParPiece(Piece* pieceCourante, Piece* piecePrecedente) {
    /*
    Actualise l'état des cases atteignables par la pièceCourante et la piècePrécédente.
    */

    // Remet à zéro l'état d'atteinte précédent
    if (piecePrecedente != NULL) {
        for (int i = 0; i < COUVERTUREMAX; i++) {
            if (piecePrecedente->casesAtteignables[i] == NULL) { break; } // On arrête le parcourt après le dernier élément non-nul du tableau
            piecePrecedente->casesAtteignables[i]->estAtteignableParPiece = false;
        }
    } 
    // Recalcul l'état d'atteinte courant
    if (pieceCourante != NULL) {
        for (int i = 0; i < COUVERTUREMAX; i++) {
            if (pieceCourante->casesAtteignables[i] == NULL) { break; } // On arrête le parcourt après le dernier élément non-nul du tableau
            pieceCourante->casesAtteignables[i]->estAtteignableParPiece = true;
        }
    }
}

void actualiseCasesAtteignablesParJoueur(Case* Echiquier[8][8], Piece* Joueur[16]) {
    /*
    Pour chaque pièces du joueur, vide puis remplis leur tableau des cases atteignables.
    */

    // Vide le tableau des cases atteignables (que la pièce soit capturée ou non)
    for (int piece = 0; piece < 16; piece++) { 
        Piece* pieceCourante = Joueur[piece];
        for (int i = 0; i < pieceCourante->longueurCasesAtteignables; i++) {
            pieceCourante->casesAtteignables[i]->estAtteignableParJoueur[pieceCourante->couleur] = 0; // Remet à zéro l'état de couverture allié
            pieceCourante->casesAtteignables[i] = NULL; 
        }
        pieceCourante->longueurCasesAtteignables = 0; 
        pieceCourante->estBloquee = false; // Remet à zéro l'état bloqué
    }
    // Rempli le tableau des cases atteignables
    for (int piece = 0; piece < 16; piece++) {
        Piece* pieceCourante = Joueur[piece]; 
        if (!(pieceCourante->estCapturee)) { 
            pieceCourante->calculAtteignable(Echiquier, pieceCourante); // Recalcul l'état de couverture allié et rempli le tableau des cases atteignables
        }
        if (pieceCourante->casesAtteignables[0] == NULL) { pieceCourante->estBloquee = true; } // Recalcul l'état bloqué
    }
}

void actualiseExposeRoi(Case* Echiquier[8][8], Piece* joueurCourant[16], Piece* joueurAdverse[16]) {
    /*
    Pour chaque pièce du joueurCourant, supprime de ses cases atteignables celles exposant son roi à l'échec.
    */

    for (int piece = 0; piece < 16; piece++) {
        Piece* pieceCourante = joueurCourant[piece];
        if (!(pieceCourante->estCapturee) && !(pieceCourante->estBloquee)) {
            for (int coup = 0; coup < pieceCourante->longueurCasesAtteignables; coup++) { // Pour chacun des coups possible par la pièce
                Case* caseCible = pieceCourante->casesAtteignables[coup];

                // Sauvegarde de la position initiale
                int xPrecedent = pieceCourante->x;
                int yPrecedent = pieceCourante->y;

                // Simulation du mouvement (et sauvegarde de la pièce capturée)
                Piece* pieceCapturee = mouvement(Echiquier, pieceCourante, caseCible, true); 
                
                // Le cas échéant, suppression de la case exposant le roi à l'échec
                actualiseCasesAtteignablesParJoueur(Echiquier, joueurAdverse);
                Case* caseRoyale = Echiquier[joueurCourant[4]->x][joueurCourant[4]->y];
                if (caseRoyale->estAtteignableParJoueur[joueurAdverse[4]->couleur] > 0) {
                    suppressionCasesAtteignables(pieceCourante, caseCible);
                    coup--;
                }
                
                // Restauration de l'état initial
                mouvement(Echiquier, pieceCourante, Echiquier[xPrecedent][yPrecedent], true); // On replace notre pièce
                if (pieceCapturee) { // Si la simulation avait capturée une pièce
                    pieceCapturee->estCapturee = false;
                    
                    if ((pieceCourante->role == PION) && (pieceCourante->vientDePrendreEnPassant)) {
                        pieceCourante->vientDePrendreEnPassant = false;
                        // On replace le pion capturé en passant
                        Echiquier[xPrecedent][caseCible->y]->piece = pieceCapturee;
                        pieceCapturee->x = xPrecedent;
                        pieceCapturee->y = caseCible->y;
                    } else {
                        // On replace la pièce capturée
                        Echiquier[caseCible->x][caseCible->y]->piece = pieceCapturee;
                        pieceCapturee->x = caseCible->x;
                        pieceCapturee->y = caseCible->y;
                    }
                } else {
                    if ((pieceCourante->role == ROI) && (pieceCourante->vientDeRoquer)) { // On a roqué
                        pieceCourante->vientDeRoquer = false;
                        // On replace la tour
                        if (caseCible->y == 6) { // C'était un petit roque
                            Piece* tourDroite = Echiquier[xPrecedent][5]->piece;
                            tourDroite->y = 7;
                            Echiquier[xPrecedent][7]->piece = tourDroite;
                            Echiquier[xPrecedent][5]->piece = NULL;
                        } else if (caseCible->y == 2) { // C'était un grand roque
                            Piece* tourGauche = Echiquier[xPrecedent][3]->piece;
                            tourGauche->y = 0;
                            Echiquier[xPrecedent][0]->piece = tourGauche;
                            Echiquier[xPrecedent][3]->piece = NULL;
                        }
                    }
                }
            }
        }
    }
}
