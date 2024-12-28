#include ".././include/affichage.h"

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
                printf("\033[0m"); // Réinitialise la couleur par défaut 
                // Pour chaque case réelle de l'echiquier
                Case* caseCourante = Echiquier[i][j];
                Piece* contenuCase = caseCourante->piece;
                for (int column = 0; column < CELL_WIDTH; column++) {
                    if (caseCourante->estSelectionnee) {
                         printf("\033[0;45m"); // On active le fond magenta
                    } else if (caseCourante->estAtteignableParPiece) { // Else if car si la case est selectionnée alors elle était atteignable
                        if (contenuCase != NULL) { // Une piece adverse est atteignable
                            printf("\033[0;41m"); // On active le fond rouge
                        } else {
                            printf("\033[0;42m"); // On active le fond vert
                        }
                    } else {
                        if (i%2 == 0) {
                            (j%2 == 0) ?  printf("\033[47m") : printf("\033[40m");;
                        } else {
                            (j%2 == 0) ?  printf("\033[40m") : printf("\033[47m");;
                        }
                    }

                    if ( (row == CELL_HEIGHT / 2) && (column == CELL_WIDTH / 2) && (contenuCase != NULL) ) { // Au centre de la case affichée
                        if (contenuCase->estSelectionnee) {
                            printf("\033[35m%s\033[0m", contenuCase->forme); // On met la piece en vert
                        } else {
                            contenuCase->couleur == BLANC ? printf("\033[34m%s\033[0m", contenuCase->forme) : printf("\033[31m%s\033[0m", contenuCase->forme); // TODO - Affiche la piece de la bonne couleur
                        }
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