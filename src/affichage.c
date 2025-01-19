#include <stdio.h> // printf()

#include ".././include/affichage.h"

void afficheEchiquier(Case* Echiquier[8][8], int scoreBlancs, int scoreNoirs) {
    /*
    Affiche l'echiquier en affichant chaque cases avec une couleur personnalisée en fonction de son état.
    */
    
    const int CELL_HEIGHT = 3; // Impair pour avoir un milieu
    const int CELL_WIDTH = CELL_HEIGHT * 2 + 1; // Le ratio largeur:hauteur ASCII est 1:2 mais j'ai besoin d'un centre

    printf("\033[2;1H"); // Déplace le curseur en haut à gauche du terminal, en laissant la place pour le timer
    printf("Echiquier :\033[K\n");

    for (int l = 0; l < 8 * CELL_WIDTH + 4; l++) {printf("\033[46m ");} // Bordure supérieure
    printf("\033[0m\033[K\n"); // Réinitialise la couleur par défaut et saute la ligne

    for (int i = 0; i < 8; i++) {
        for (int row = 0; row < CELL_HEIGHT; row++) { // Pour chaque lignes de l'affichage d'une unique case
            printf("\033[46m  \033[0m\033[K"); // Bordure gauche
            
            for (int j = 0; j < 8; j++) {
                printf("\033[0m"); // Réinitialise la couleur par défaut 
                
                // --- Pour chaque cases de l'echiquier --- //
                Case* caseCourante = Echiquier[i][j];
                Piece* contenuCase = caseCourante->piece;

                for (int column = 0; column < CELL_WIDTH; column++) {
                    if (caseCourante->estSelectionnee) {
                         printf("\033[0;45m"); // On active le fond magenta
                    } else if (caseCourante->estAtteignableParPiece) { // else if car si la case est selectionnée alors elle était atteignable
                        if (contenuCase != NULL) { // Une piece adverse est atteignable
                            printf("\033[0;41m"); // On active le fond rouge
                        } else {
                            printf("\033[0;42m"); // On active le fond vert
                        }
                    } else { // Sinon on affiche la case normalement
                        if (i%2 == 0) {
                            (j%2 == 0) ?  printf("\033[47m") : printf("\033[40m");;
                        } else {
                            (j%2 == 0) ?  printf("\033[40m") : printf("\033[47m");;
                        }
                    }

                    if ((contenuCase != NULL) && (column == CELL_WIDTH / 2)) { // Au centre de l'affichage d'une case pleine
                        if (row == CELL_HEIGHT / 2) {
                            if (contenuCase->estSelectionnee) {
                                printf("\033[35m%s\033[0m", contenuCase->forme); // On met la piece en vert
                            } else {
                                contenuCase->couleur == BLANC ? printf("\033[34m%s\033[0m", contenuCase->forme) : printf("\033[31m%s\033[0m", contenuCase->forme); // TODO - Affiche la piece de la bonne couleur
                            }
                        } else if (row == CELL_HEIGHT / 2 + 1) { // Valeur en dessous de la pièce
                                contenuCase->couleur == BLANC ? printf("\033[34m%d\033[0m", (int)contenuCase->role) : printf("\033[31m%d\033[0m", (int)contenuCase->role);
                        } else {
                            printf(" ");
                        }
                    } else {
                        printf(" ");
                    }
                }
                printf("\033[0m"); // Réinitialise la couleur
            }
            
            printf("\033[46m  \033[0m\033[K\n"); // Bordure droite
        }
    }

    for (int l = 0; l < 8 * CELL_WIDTH + 4; l++) {printf("\033[46m ");} // Bordure inférieure

    // 26 lignes echiquier + 2 lignes (temps restant et echiquier)

    printf("\033[0m\033[K\n"); // Réinitialise la couleur par défaut et saute la ligne
    printf("SCORE\033[K\nBlancs : %d\033[K\nNoirs : %d\033[K\n", scoreBlancs, scoreNoirs);
    printf("\033[J");

    // 28 lignes + 3 score
}

