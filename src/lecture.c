#include <stdio.h> // printf()

#include ".././include/lecture.h"

void deplacerCurseur(int x, int y) {
    printf("\033[%d;%dH", x, y);
}

/*========= Le code ci-dessous à été généré par ChatGPT =========*/

// Fonction pour désactiver le mode tamponné (entrée immédiate)
void set_terminal_raw_mode() {
    struct termios t;

    // Récupérer les paramètres actuels du terminal
    tcgetattr(STDIN_FILENO, &t);

    // Désactiver l'écho et le mode canonique
    t.c_lflag &= ~(ICANON | ECHO);

    // Appliquer les nouveaux paramètres
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

// Fonction pour rétablir les paramètres par défaut du terminal
void reset_terminal_mode(struct termios *orig) {
    tcsetattr(STDIN_FILENO, TCSANOW, orig);
}