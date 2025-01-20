#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

#include "piece.h"
#include "echiquier.h"

int sauvegarderEchiquier(Piece* joueurBlanc[16], int scoreBlancs, Piece* joueurNoir[16], int scoreNoirs, int couleurJoueurCourant, char* dossierDeSauvegarde);
int chargerEchiquier(Case* Echiquier[8][8], Piece* joueurBlanc[16], int* scoreBlancs, Piece* joueurNoir[16], int* scoreNoirs, char* dossierDeSauvegarde);

#endif // SAUVEGARDE_H