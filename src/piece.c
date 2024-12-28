#include <stdlib.h> // malloc(), free(), exit()

#include ".././include/calcul_atteignable.h" // calculAtteignablePion(), calculAtteignableCavalier(), calculAtteignableTour(), calculAtteignableFou(), calculAtteignableReine(), calculAtteignableRoi()

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
    if (i >= COUVERTUREMAX) { exit(69); } // Il y davantage de cases atteignables qu'il y a de cases dans le plateau.
    pieceCourante->casesAtteignables[i] = caseAtteignable;
    
    // On met à jour les statuts de la pièce
    pieceCourante->longueurCasesAtteignables++;
    pieceCourante->estBloquee = false; // À partir du moment où la fonction est appelée, la pièce n'est plus bloquée (inutile car déjà effectué dans la fonction d'actualisation)

    caseAtteignable->estAtteignableParJoueur[pieceCourante->couleur]++; // La case est donc atteignable par une pièce alliée de plus
}

 void suppressionCasesAtteignables(Piece* pieceCourante, Case* caseSupprimable) {
    /*
    Supprime caseSupprimable dans le tableau de casesAtteignables de pieceCourante
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

    caseSupprimable->estAtteignableParJoueur[pieceCourante->couleur]--;
 }

void actualiseCasesAtteignablesParPiece(Piece* pieceCourante, Piece* piecePrecedente) {
    /*
    Vide puis remplis le tableau des cases atteignables de pieceActualisable.

    PRÉCONDITION : pieceActualisable n'est pas bloquée ni capturée
    */
    if (piecePrecedente != NULL) {
        for (int i = 0; i < COUVERTUREMAX; i++) {
            if (piecePrecedente->casesAtteignables[i] == NULL) { break; } // On arrête le parcourt après le dernier élément non-nul du tableau
            piecePrecedente->casesAtteignables[i]->estAtteignableParPiece = false;
        }
    } 
    if (pieceCourante != NULL) {
        for (int i = 0; i < COUVERTUREMAX; i++) {
            if (pieceCourante->casesAtteignables[i] == NULL) { break; } // On arrête le parcourt après le dernier élément non-nul du tableau
            pieceCourante->casesAtteignables[i]->estAtteignableParPiece = true;
        }
    }
}

void actualiseCasesAtteignablesParJoueur(Case* Echiquier[8][8], Piece* Joueur[16]) {
    /*
    Vide puis remplis le tableau des cases atteignables de pieceActualisable.
    */
    // Vide les cases atteignables (que la pièce soit capturée ou non)
    for (int piece = 0; piece < 16; piece++) { 
        Piece* pieceCourante = Joueur[piece];
        for (int i = 0; i < pieceCourante->longueurCasesAtteignables; i++) {
            pieceCourante->casesAtteignables[i]->estAtteignableParJoueur[pieceCourante->couleur] = 0; // Remet à zéro le status allié
            pieceCourante->casesAtteignables[i] = NULL; // Vide le tableau
        }
        pieceCourante->longueurCasesAtteignables = 0; // Remet à zéro la longueur du tableau
        pieceCourante->estBloquee = false; // Remet à zéro le statut bloquée
    }
    // Rempli les cases atteignables
    for (int piece = 0; piece < 16; piece++) {
        Piece* pieceCourante = Joueur[piece];
        if (!(pieceCourante->estCapturee)) { // N'actualise que les pièces encore en jeu
            pieceCourante->calculAtteignable(Echiquier, pieceCourante); // Recalcule le statut allié et rempli le tableau
        }
        if (pieceCourante->casesAtteignables[0] == NULL) { pieceCourante->estBloquee = true; }
    }
}

void actualiseExposeRoi(Case* Echiquier[8][8], Piece* joueurCourant[16], Piece* joueurAdverse[16]) {
    /*
    Supprime les cases qui mettent en échec le roi des tableaux de cases atteignables des pièces.
    */
    for (int piece = 0; piece < 16; piece++) { // Pour chacune des pièces du joueur
        Piece* pieceCourante = joueurCourant[piece];
        if (!(pieceCourante->estCapturee) && !(pieceCourante->estBloquee)) { // N'actualise que les pièces encore en jeu et non bloquées 
            for (int coup = 0; coup < pieceCourante->longueurCasesAtteignables; coup++) { // Pour chacun des coups possible par la pièce
                Case* caseCourante = pieceCourante->casesAtteignables[coup]; // Forcément non-nulle car la fonction est appelée juste après actualisation des tableaux de cases atteignables
                int xPrecedent = pieceCourante->x;
                int yPrecedent = pieceCourante->y;

                int xCible = caseCourante->x;
                int yCible = caseCourante->y;
                Piece* contenuCaseCible = caseCourante->piece; // On garde en mémoire la pièce présente sur la case cible

                // Simulation du coup
                Echiquier[xPrecedent][yPrecedent]->piece = NULL; // On enlève la pièce courante de sa positon précédente
                if (contenuCaseCible != NULL) { contenuCaseCible->estCapturee = true; } // On capture la pièce si elle existe (on a seulement accès à des cases adverses)
                caseCourante->piece = pieceCourante; // Écrase l'adrese de la pièce capturée avec celle de la pièce alliée
                pieceCourante->x = xCible; // On met à jours les coordonnées de la pièce déplacée
                pieceCourante->y = yCible;

                // On actualise les cases atteignables par l'adversaire après le mouvement potentiel
                actualiseCasesAtteignablesParJoueur(Echiquier, joueurAdverse); 
                
                // Interdiction de mettre en échec
                Case* caseRoyale = Echiquier[joueurCourant[4]->x][joueurCourant[4]->y];
                if (caseRoyale->estAtteignableParJoueur[joueurAdverse[4]->couleur] > 0) {
                    suppressionCasesAtteignables(pieceCourante, caseCourante);
                    coup--; // On reprend le même indice car la suppression décale le tableau des cases atteignables 
                    //! S'assurer que pieceCourante->longueurCasesAtteignables suive et ne soit pas fixe sinon faudra passer par un while
                } 

                // Retour à la position initiale
                pieceCourante->x = xPrecedent;
                pieceCourante->y = yPrecedent;
                caseCourante->piece = contenuCaseCible;
                if (contenuCaseCible != NULL) { contenuCaseCible->estCapturee = false; } // On annule la capture si elle a eu lieu
                Echiquier[xPrecedent][yPrecedent]->piece = pieceCourante; // On annule le mouvement
            }
        }
    }

}