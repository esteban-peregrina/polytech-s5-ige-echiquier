#include <stdbool.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

#include ".././headers/lecture.h"
#include ".././headers/piece.h"
#include ".././headers/echiquier.h"

Case* creationCase(int couleur) {
    /*
    Renvoie l'adresse d'une case vide dont la couleur est specifiée.
    */

    Case* caseCree = NULL;
    caseCree = malloc(sizeof(Case));
    if (caseCree == NULL) { exit(EXIT_FAILURE); }
    
    caseCree->estAtteignable = false;
    caseCree->estSelectionnee = false;

    return caseCree;
}

void initialiseEchiquier(Case* Echiquier[8][8]) {
    /*
    Rempli l'echiquier de cases vides aux couleurs alternées.
    */

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (i%2 == 0) {
                Echiquier[i][j] = (j%2 == 0) ? creationCase(BLANC) : creationCase(NOIR);
            } else {
                Echiquier[i][j] = (j%2 == 0) ? creationCase(NOIR) : creationCase(BLANC);
            }
        }
    }
}

void videEchiquier(Case* Echiquier[8][8]) {
    /*
    Libère la mémoire allouée pour les cases.
    */

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            free(Echiquier[i][j]);
            Echiquier[i][j] = NULL; // Libère la mémoire allouée pour les cases
        }
    }
}

void afficheEchiquier(Case* Echiquier[8][8]) {
    /*
    Affiche l'echiquier.
    */

    const int CELL_HEIGHT = 3; // Impair pour avoir un milieu
    const int CELL_WIDTH = CELL_HEIGHT * 2 + 1; // Le ratio largeur:hauteur ASCII est 1:2 mais j'ai besoin d'un centre
    
   //printf("\033[H\033[J"); // Vide le terminal
    printf("Echiquier :\n");

    for (int l = 0; l < 8 * CELL_WIDTH + 4; l++) {printf("\033[46m ");} // Bordure supérieure
    printf("\033[0m \n"); // Réinitialise la couleur par défaut et saute la ligne

    for (int i = 0; i < 8; i++) {
        for (int row = 0; row < CELL_HEIGHT; row++) { // Chaque ligne de la case affichée
            printf("\033[46m  "); // Bordure gauche
            for (int j = 0; j < 8; j++) {
                // Pour chaque case réelle de l'echiquier
                Case* caseCourante = Echiquier[i][j];
                Piece* contenuCase = caseCourante->piece;
                for (int column = 0; column < CELL_WIDTH; column++) {
                    if (caseCourante->estAtteignable == true) {
                        if (contenuCase != NULL) { // Une piece adverse est atteignable
                            printf("\033[0;41m"); // On active le fond rouge
                        } else {
                            printf("\033[0;42m"); // On active le fond vert
                        }
                        
                    } else if ( (caseCourante->piece != NULL) && (caseCourante->piece->estSelectionnee == true) ) {
                        printf("\033[0;44m"); // On active le fond bleu
                    } else {
                        if (i%2 == 0) {
                            (j%2 == 0) ?  printf("\033[47m") : printf("\033[40m");;
                        } else {
                            (j%2 == 0) ?  printf("\033[40m") : printf("\033[47m");;
                        }
                    }

                    if ( (row == CELL_HEIGHT / 2) && (column == CELL_WIDTH / 2) && (contenuCase != NULL) ) { // Au centre de la case affichée
                        contenuCase->couleur == BLANC ? printf("\033[34m%s\033[0m", contenuCase->forme) : printf("\033[31m%s\033[0m", contenuCase->forme); // TODO - Affiche la piece de la bonne couleur
                    } else {
                        printf(" ");
                    }
                }
                printf("\033[0m"); // Réinitialise la couleur
            }
            printf("\033[46m  \033[0m\n"); // Bordure droite
        }
    }

    for (int l = 0; l < 8 * CELL_WIDTH + 4; l++) {printf("\033[46m ");} // Bordure inférieure
    printf("\033[0m\n"); // Réinitialise la couleur par défaut et saute la ligne
}

void partieEchec() {
    /*
    Démarre une partie d'échec.
    */
    
    // TODO - Proposer de charger une sauvegarde
    // TODO - Proposer de jouer contre un joueur ou contre une IA

    Case* Echiquier[8][8]; // Déclaration du Echiquier
    initialiseEchiquier(Echiquier); // Initialisation de l'echiquier (chaque case est vide, non selectionnée ni atteignable, et de la bonne couleur)
    Piece *Blancs[16], *Noirs[16];
    initialiseJoueur(Echiquier, Blancs, BLANC); 
    initialiseJoueur(Echiquier, Noirs, NOIR);
    bool enEchec = false;
    Piece** joueurCourant = Blancs; // Les blancs commences
    Piece** joueurAdverse = Noirs;

    while (!enEchec) {
        for (int i = 0; i < 16; i++) { actualiseCasesAtteignables(Echiquier, joueurAdverse, joueurCourant[4], joueurCourant[i]); } // On actualise chaque pièce du joueur

        bool aJoue = false;
        Menu menu = PIECES; // On commence par sélectionner une pièce
        int indicePieceCourante = 0;
        Piece* pieceCourante; // Une fois initialisé, les éléments du tableau ne sont jamais nul.
        int indiceCaseCourante = 0;
        Case* caseCourante;
        // ! - Penser à free les cases et les pieces

        while (!aJoue) {
            // Sauvegarder les paramètres originaux du terminal
            struct termios orig_termios;
            tcgetattr(STDIN_FILENO, &orig_termios);

            // Configurer le terminal en mode "raw"
            set_terminal_raw_mode();

            printf("Sélectionnez une pièce à jouer à l'aide des touches directionnelles (appuyez sur 'q' pour quitter) :");

            char actionJoueur;
            if (read(STDIN_FILENO, &actionJoueur, 1) != 1) { exit(EXIT_FAILURE); } // Écrit l'entrée utilisateur lue dans &actionJoueur et vérifie que cela à fonctionné

            if (menu == PIECES) {
                if (actionJoueur == '\033') {
                    char seq[2];
                    if (read(STDIN_FILENO, &seq[0], 1) != 1 || read(STDIN_FILENO, &seq[1], 1) != 1) { exit(EXIT_FAILURE); }

                    if (seq[0] == '[') {
                        switch (seq[1]) {
                            case 'A':
                                printf("Flèche Haut\n");
                                break;
                            case 'B': // Flèche bas
                                printf("Flèche bas\n");
                                indiceCaseCourante = 0;
                                caseCourante = pieceCourante->casesAtteignables[indiceCaseCourante];  //! - Ne verifie pas si ce premier élément est nul
                                caseCourante->estSelectionnee = true;
                                pieceCourante->estSelectionnee = true;
                                menu = COUPS;
                                break;
                            case 'C': // Flèche droite
                                printf("Flèche droite\n");
                                pieceCourante = joueurCourant[indicePieceCourante++]; //! - If tout au bout ou piece capturée (~= NULL)
                                break;
                            case 'D': // Flèche gauche
                                printf("Flèche gauche\n");
                                pieceCourante = joueurCourant[indicePieceCourante--]; //! - If tout au bout ou piece capturée (~= NULL)
                                break;
                            default:
                                printf("Caractère incorrect\n");
                                break;
                        }
                    } else {
                        printf("Autre caractère spécial\n");
                    }
                }
                afficheEchiquier(Echiquier);
            } else { //menu == COUPS
                /*
                if (menu != PIECES && actionJoueur == 'a') { // On retourne au choix des pièces (pas besoin de réinitialiser l'indice)
                    caseCourante->estSelectionnee = false;
                    menu = PIECES;
                } else if (actionJoueur == 'd') { // <-
                    caseCourante->estSelectionnee = false;
                    caseCourante = pieceCourante->casesAtteignables[indiceCaseCourante--]; //! - If tout au bout ou case NULL
                    caseCourante->estSelectionnee = true;
                } else if (actionJoueur == 'c') { // ->
                    caseCourante->estSelectionnee = false;
                    caseCourante = pieceCourante->casesAtteignables[indiceCaseCourante++]; //! - If tout au bout ou case NULL
                    caseCourante->estSelectionnee = true;
                } else if (actionJoueur == '\n') { // Le coup est validé
                    // TODO - Mettre à jour les coordonnées de la pièce
                    // TODO - Détruire la pièce ennemie si il y en a une (suppression de la liste des pièces de l'adversaire) ON SUPPOSE QUE LES CASES ATTEIGNABLES SONT VALIDES

                    aJoue = true;
                    afficheEchiquier(Echiquier);
                }
                */
            }

            // Rétablir les paramètres originaux
            reset_terminal_mode(&orig_termios);
        }

        videEchiquier(Echiquier); // Libère les cases et les pièces
    }
}