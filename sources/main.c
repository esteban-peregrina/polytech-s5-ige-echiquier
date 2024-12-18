#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include ".././headers/piece.h"
#include ".././headers/echiquier.h"

int main() {

    // Test de l'affichage du Plateau vide
    Case* Plateau[8][8]; // Déclaration du Plateau
    initialisePlateau(Plateau); // Initialisation du plateau (chaque case est vide, non selectionnée ni atteignable, et de la bonne couleur)
    affichePlateau(Plateau);
    videPlateau(Plateau);

    
    return 0;
}