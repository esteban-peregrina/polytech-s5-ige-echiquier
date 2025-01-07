#include ".././include/echiquier.h"
#include ".././include/timer.h"

// TODO - Tests unitaire ?
// TODO - Tests valgrind
// TODO - Créer des grilles de poids pour chaque pièce pour adapter leur valeur en fonction de leur position (puis IA ?) + facteur temporel (les heatmaps sont pas valable à la fin)
// TODO - IA
// TODO - Découper la fonciton "partiEchec()" et créer une fonction "tourDuJoueur()" pour accuiellir l'IA (la différence c'est qu'on donne pas la main au joueur entre deux tours)
// TODO - Modifier heatmap + valeur lors de la promotion
// TODO - Un truc pour aider à regler la taille du terminal au bon endroit

int main() {
    jeuEchec();

    return 0;
}
