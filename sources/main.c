#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#include ".././headers/piece.h"
#include ".././headers/echiquier.h"

// TODO - Exit codes specifique
// TODO - Utiliser le tableau de piece comme le tableau de cases ? (simplifie selection quand menu == pieces)
// TODO - Ne pas afficher toutes les cases atteignables, mais à chaque fois, parcourir tout les cases de la pièce selectionnée pour mettre a jour
// TODO - Peut-etre creer un tab de deux bool "estAtteignableParJoueur[2]" (sert a l'exposition ?) vs "estAtteignableParPieceSelectionnee"
// TODO - Le bool estCapturé est bien pour eviter d'avoir un tableau Joueur avec des element NULL (comme elements de echiquier qui sont jamais null) mais il faut 

int main() {
    partieEchec();

    printf("Programme terminé.\n");
    return 0;
}
