#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include ".././headers/piece.h"
#include ".././headers/echiquier.h"

int main() {

    // Test de l'affichage du Echiquier vide
    Case* Echiquier[8][8]; // Déclaration du Echiquier
    initialiseEchiquier(Echiquier); // Initialisation du Echiquier (chaque case est vide, non selectionnée ni atteignable, et de la bonne couleur)
    Echiquier[0][1]->estAtteignable = true;
    Echiquier[1][1]->estAtteignable = true;
    Echiquier[1][0]->estAtteignable = true;
    Echiquier[0][0]->estSelectionne = true;
    afficheEchiquier(Echiquier);
    videEchiquier(Echiquier);


    return 0;
}