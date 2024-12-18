#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include ".././headers/piece.h"
#include ".././headers/echiquier.h"

Case* creationCase(int couleur) {
    /*
    Renvoie l'adresse d'une case vide dont la couleur est specifiée.
    */

    Case* Case = NULL;
    Case = malloc(sizeof(Case));
    if (Case == NULL) { exit(EXIT_FAILURE); }
    
    Case->couleur = couleur;
    Case->estSelectionne = false;
    Case->estAtteignable = false;

    Case->caseAtteignableSuivante = NULL;
    Case->caseAtteignablePrecedente = NULL;

    return Case;
}

void destructionCase(Case* Case) { // Nécessaire ?
    /*
    Libère la case pointée.  
    */

    if (Case == NULL) { exit(EXIT_FAILURE); } // On ne devrait pas passer de Case vide à cette fonction
    free(Case);
}

void insertionListeCasesAtteignables(Case* caseAtteignable, ListeCasesAtteignables* listeCasesAtteignables) {
    /*
    Insère caseAtteignable en tete de listeCasesAtteignables
    */

    Case* temp = listeCasesAtteignables->tete;
    if (temp != NULL) { temp->caseAtteignablePrecedente = NULL; }
    listeCasesAtteignables->tete = caseAtteignable;
    caseAtteignable->caseAtteignableSuivante = temp;
    caseAtteignable->caseAtteignablePrecedente = NULL;
}

void initialisePlateau(Case* Plateau[8][8]) {
    /*
    Rempli le plateau de cases vides aux couleurs alternées.
    */

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (i%2 == 0) {
                Plateau[i][j] = (j%2 == 0) ? creationCase(BLANC) : creationCase(NOIR);
            } else {
                Plateau[i][j] = (j%2 == 0) ? creationCase(NOIR) : creationCase(BLANC);
            }
        }
    }
}

void videPlateau(Case* Plateau[8][8]) {
    /*
    Rempli le plateau de cases vides aux couleurs alternées.
    */

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            destructionCase(Plateau[i][j]);
            Plateau[i][j] = NULL;
        }
    }
}

void affichePlateau(Case* Plateau[8][8]) {
    /*
    Affiche le plateau.
    */

    const int CELL_WIDTH = 7;  
    const int CELL_HEIGHT = 3;

    printf("Plateau :\n");

    for (int i = 0; i < 8; i++) {
        for (int row = 0; row < CELL_WIDTH; row++) { // Chaque ligne de la case affichée
            for (int j = 0; j < 8; j++) {
                // Pour chaque case réelle du plateau
                Case* caseCourante = Plateau[i][j];
                Piece* contenuCase = caseCourante->piece;
                caseCourante->couleur == BLANC ? printf("\033[47m") : printf("\033[40m");
                
                //TODO - Print un truc en 7*3 avec la piece en son centre

                for (int column = 0; column < CELL_WIDTH; column++) {
                    if ( (row == CELL_HEIGHT / 2) && (column == CELL_WIDTH / 2) && (contenuCase->role != VIDE) ) { // Au centre de la case affichée
                        contenuCase->couleur == BLANC ? printf("\033[37m%c", contenuCase->forme) : printf("\033[30m%c", contenuCase->forme); // Affiche la piece de la bonne couleur
                    } else {
                        printf(" "); 
                    }
                }
                printf("\033[0m "); // Réinitialise et permet de laisser une colonne vide
            }
            printf("\n");
        }
        printf("\n\n"); // Retourne à la ligne et laisse une ligne vide
    }
}

void partieEchec() {
    /*
    Démarre une partie d'échec.
    */
    
    // TODO - Proposer de charger une sauvegarde
    // TODO - Proposer de jouer contre un joueur ou contre une IA

    Case* Plateau[8][8]; // Déclaration du Plateau
    initialisePlateau(Plateau); // Initialisation du plateau (chaque case est vide, non selectionnée ni atteignable, et de la bonne couleur)
    Piece* Joueurs[2];
    //TODO - initialiseJoueur(Joueurs[2]);; // Déclaration des pièces des 2 joueurs, le roi est stocké comme tete de liste circulaire dans le tableau, index 0 pour Noir et 1 pour Blanc
    bool enEchec = false;
    int joueur = NOIR;

    while (!enEchec) {
        affichePlateau(Plateau); // TODO - Renommer car affiche aussi le menu (différent selon si piece ou coups)
        bool aJoue = false;
        Menu menu = PIECES;
        joueur = (joueur == NOIR) ? BLANC : NOIR; // On commute de joueur
        Piece* pieceCourante = Joueurs[joueur]; // On commence par les blancs (comme par hasard)
        
        while (!aJoue) {
            printf("Sélectionnez une pièce à déplacer, puis validez avec Entrée");
            char actionJoueur = getchar();

            if (actionJoueur == '\033') {
                Case* caseCourante = pieceCourante->casesAtteignables->tete;

                for (int i = 0; i < 2; i++) { actionJoueur = getchar(); }

                if (menu != COUPS && actionJoueur == 'B') { // La pièce est sélectionnée
                    caseCourante->estSelectionne = true;
                    menu = COUPS;
                } else if (menu != PIECES && actionJoueur == 'A') { // On retourne au choix des pièces
                    caseCourante->estSelectionne = false;
                    menu = PIECES;
                } else if (actionJoueur == 'D' || actionJoueur == 'C') { // Si c'est <- ou ->
                    if (menu == PIECES) {
                        if (actionJoueur == 'D') { // <-
                            pieceCourante = pieceCourante->piecePrecedente;
                        } else { // ->
                            pieceCourante = pieceCourante->pieceSuivante;
                        }
                        //TODO - actualiseCasesAtteignables(pieceCourante, Plateau);
                        affichePlateau(Plateau);

                    } else { // menu == COUPS
                        if (actionJoueur == 'D') { // <-
                            caseCourante->estSelectionne = false;
                            caseCourante = caseCourante->caseAtteignablePrecedente;
                            caseCourante->estSelectionne = true;
                        } else { // ->
                            caseCourante->estSelectionne = false;
                            pieceCourante = caseCourante->caseAtteignableSuivante;
                            caseCourante->estSelectionne = true;
                        }
                        affichePlateau(Plateau);
                    }
                }
                
            } else if (menu == COUPS && actionJoueur == '\n') { // Le coup est validé
                // TODO - Mettre à jour les coordonnées de la pièce
                // TODO - Détruire la pièce ennemie si il y en a une (suppression de la liste des pièces de l'adversaire) ON SUPPOSE QUE LES CASES ATTEIGNABLES SONT VALIDES

                aJoue = true;
            }
        }
    }

}