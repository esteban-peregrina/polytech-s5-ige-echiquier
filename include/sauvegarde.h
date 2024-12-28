#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

#include <stdio.h> // fopen, fclose, fprintf
#include <sys/stat.h> // mkdir
#include <unistd.h> // chdir

#include "piece.h"
#include "echiquier.h"

int sauvegarderEchiquier(Piece* joueurBlanc[16], Piece* joueurNoir[16], int couleurJoueurCourant, char* dossierDeSauvegarde);
int chargerEchiquier(Case* Echiquier[8][8], Piece* joueurBlanc[16], Piece* joueurNoir[16],  int* couleurJoueurCourant, char* dossierDeSauvegarde);

#endif // SAUVEGARDE_H