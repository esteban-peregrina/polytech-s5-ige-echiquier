#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

#include <stdio.h> // fopen, fclose, fprintf
#include <sys/stat.h> // mkdir
#include <unistd.h> // chdir

#include ".././headers/piece.h"
#include ".././headers/echiquier.h"

void save_echiquier(Case* Echiquier[8][8], Piece* joueurBlanc[16], Piece* joueurNoir[16], char* directoryname);

#endif // SAUVEGARDE_H