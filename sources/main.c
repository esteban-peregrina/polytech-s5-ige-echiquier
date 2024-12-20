#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#include ".././headers/piece.h"
#include ".././headers/echiquier.h"

int main() {
    partieEchec();

    printf("Programme terminé.\n");
    return 0;
}
