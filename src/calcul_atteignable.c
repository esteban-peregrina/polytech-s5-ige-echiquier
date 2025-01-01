#include "./../include/calcul_atteignable.h"

void calculAtteignablePion(Case* Echiquier[8][8], Piece* self) {
    /*
    Met à jour le tableau des cases atteignables d'un pion.
    */

    int origine, droite, avant, bordAvant, bordDroite, bordGauche, rangeeEnPassant;
    switch (self->couleur) {
        case BLANC :
            origine = 1;
            droite = 1;
            avant = 1;
            bordAvant = 7;
            bordDroite = 7;
            bordGauche = 0;
            rangeeEnPassant = 4;
            break;
        case NOIR :
            origine = 6;
            droite = -1;
            avant = -1;
            bordAvant = 0;
            bordDroite = 0;
            bordGauche = 7;
            rangeeEnPassant = 3;
            break;
    }

    // Double pas
    if (self->x == origine) { // Le pion est sur sa rangée initiale (comme il ne peut jamais revenir en arrière il n'y a pas de vérifier qu'on joue son tout premier mouvement)
        if ((!Echiquier[self->x + 1 * avant][self->y]->piece) && (!Echiquier[self->x + 2 * avant][self->y]->piece)) { // Il n'y a pas de piece devant
            insertionCasesAtteignables(self, Echiquier[self->x + 2 * avant][self->y]);
        }
    }
    // Pas simple
    if (self->x != bordAvant) { 
        // Avancer tout droit
        if (!Echiquier[self->x + avant][self->y]->piece) { insertionCasesAtteignables(self, Echiquier[self->x + avant][self->y]); }
        // Prendre en diagonale droite
        if (self->y != bordDroite) {
            Piece* pieceDiagonaleDroite = Echiquier[self->x + avant][self->y + droite]->piece;
            if ((pieceDiagonaleDroite) && 
                (pieceDiagonaleDroite->couleur != self->couleur) && 
                (!pieceDiagonaleDroite->estCapturee)) 
            { insertionCasesAtteignables(self, Echiquier[self->x + avant][self->y + droite]); }
        }
        // Prendre en diagonale gauche
        if (self->y != bordGauche) {
            Piece* pieceDiagonaleGauche = Echiquier[self->x + avant][self->y - droite]->piece;
            if ((pieceDiagonaleGauche) && 
                (pieceDiagonaleGauche->couleur != self->couleur) &&
                (!pieceDiagonaleGauche->estCapturee)) 
            { insertionCasesAtteignables(self, Echiquier[self->x + avant][self->y - droite]); }

        }
    }

    // Prise en passant
    if (self->x == rangeeEnPassant) {
        // Vérifie les pions adjacents
        if (self->y != bordGauche) { // Prise à gauche
            Piece* pieceGauche = Echiquier[self->x][self->y-droite]->piece;
            if ((pieceGauche) && 
                (pieceGauche->role == PION) && 
                (pieceGauche->couleur != self->couleur) &&
                (!pieceGauche->estCapturee) &&
                (pieceGauche->vientDeFaireDoublePas)) 
            { insertionCasesAtteignables(self, Echiquier[self->x + avant][self->y - droite]); }
        }
        if (self->y != bordDroite) { // Prise à droite 
            Piece* pieceDroite = Echiquier[self->x][self->y+droite]->piece;
            if ((pieceDroite) && 
                (pieceDroite->role == PION) && 
                (pieceDroite->couleur != self->couleur) &&
                (!pieceDroite->estCapturee) &&
                (pieceDroite->vientDeFaireDoublePas)) 
            { insertionCasesAtteignables(self, Echiquier[self->x + avant][self->y + droite]); }
        }
    }
}

void calculAtteignableCavalier(Case* Echiquier[8][8], Piece* self) {
    /*
    Met à jour le tableau des cases atteignables d'un cavalier.
    */

    int mouvements[8][2] = { {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2} }; // Ensemble des mouvements possibles pour un cavalier (coordonnées relatives)
    for (int i = 0; i < 8; i++) { // Pour chacun des mouvements
        int xCible = self->x + mouvements[i][0];
        int yCible = self->y + mouvements[i][1];

        if (xCible >= 0 && xCible < 8 && yCible >= 0 && yCible < 8) { // La case est dans les limites du plateau
            if ((!Echiquier[xCible][yCible]->piece) || (Echiquier[xCible][yCible]->piece->couleur != self->couleur)) {
                insertionCasesAtteignables(self, Echiquier[xCible][yCible]);
            }
        }
    }
}

void calculAtteignableTour(Case* Echiquier[8][8], Piece* self) {
    /*
    Met à jour le tableau des cases atteignables d'une tour.
    */

    int avant, droite, limiteAvant, limiteArriere, limiteDroite, limiteGauche;
    switch (self->couleur) {
        case BLANC :
            avant = 1;
            droite = 1;
            limiteAvant = 8;
            limiteArriere = -1;
            limiteDroite = 8;
            limiteGauche = -1;
            break;
        case NOIR :
            avant = -1;
            droite = -1;
            limiteAvant = -1;
            limiteArriere = 8;
            limiteDroite = -1;
            limiteGauche = 8;
            break;
    }

    // Déplacement avant
    int xCourant = self->x + avant;
    while (xCourant != limiteAvant) { // La case est dans la limite avant
        if (Echiquier[xCourant][self->y]->piece) { // La case n'est pas vide
            if (Echiquier[xCourant][self->y]->piece->couleur != self->couleur) { // La case contient une pièce ennemie
                insertionCasesAtteignables(self, Echiquier[xCourant][self->y]);
            }
            break; // On arrête la boucle à la rencontre d'une pièce
        } else { // La case est vide
            insertionCasesAtteignables(self, Echiquier[xCourant][self->y]);
            xCourant += avant;
        }
    }
    // Déplacement arrière
    xCourant = self->x - avant;
    while (xCourant != limiteArriere) { // La case est dans la limite avant
        if (Echiquier[xCourant][self->y]->piece) { // La case n'est pas vide
            if (Echiquier[xCourant][self->y]->piece->couleur != self->couleur) { // La case contient une pièce ennemie
                insertionCasesAtteignables(self, Echiquier[xCourant][self->y]);
            }
            break; // On arrête la boucle à la rencontre d'une pièce
        } else { // La case est vide
            insertionCasesAtteignables(self, Echiquier[xCourant][self->y]);
            xCourant -= avant;
        }
    }
    // Déplacement droite
    int yCourant = self->y + droite;
    while (yCourant != limiteDroite) { // La case est dans la limite avant
        if (Echiquier[self->x][yCourant]->piece) { // La case n'est pas vide
            if (Echiquier[self->x][yCourant]->piece->couleur != self->couleur) { // La case contient une pièce ennemie
                insertionCasesAtteignables(self, Echiquier[self->x][yCourant]);
            }
            break; // On arrête la boucle à la rencontre d'une pièce
        } else { // La case est vide
            insertionCasesAtteignables(self, Echiquier[self->x][yCourant]);
            yCourant += droite;
        }
    }
    // Déplacement gauche
    yCourant = self->y - droite;
    while (yCourant != limiteGauche) { // La case est dans la limite avant
        if (Echiquier[self->x][yCourant]->piece) { // La case n'est pas vide
            if (Echiquier[self->x][yCourant]->piece->couleur != self->couleur) { // La case contient une pièce ennemie
                insertionCasesAtteignables(self, Echiquier[self->x][yCourant]);
            }
            break; // On arrête la boucle à la rencontre d'une pièce
        } else { // La case est vide
            insertionCasesAtteignables(self, Echiquier[self->x][yCourant]);
            yCourant -= droite;
        }
    }
}

void calculAtteignableFou(Case* Echiquier[8][8], Piece* self) {
    /*
    Met à jour le tableau des cases atteignables d'un fou.
    */

    int avant, droite, limiteAvant, limiteArriere, limiteDroite, limiteGauche;
    switch (self->couleur) {
        case BLANC :
            avant = 1;
            droite = 1;
            limiteAvant = 8;
            limiteArriere = -1;
            limiteDroite = 8;
            limiteGauche = -1;
            break;
        case NOIR :
            avant = -1;
            droite = -1;
            limiteAvant = -1;
            limiteArriere = 8;
            limiteDroite = -1;
            limiteGauche = 8;
            break;
    }

    // Déplacement diagonale avant droite
    int xCourant = self->x + avant;
    int yCourant = self->y + droite;
    while (xCourant != limiteAvant && yCourant != limiteDroite) { // La case est dans la limite avant droite
        if (Echiquier[xCourant][yCourant]->piece) { // La case n'est pas vide
            if (Echiquier[xCourant][yCourant]->piece->couleur != self->couleur) { // La case contient une pièce ennemie
                insertionCasesAtteignables(self, Echiquier[xCourant][yCourant]);
            }
            break; // On arrête la boucle à la rencontre d'une pièce
        } else { // La case est vide
            insertionCasesAtteignables(self, Echiquier[xCourant][yCourant]);
            xCourant += avant;
            yCourant += droite;
        }
    }
    // Déplacement diagonale arrière droite
    xCourant = self->x - avant;
    yCourant = self->y + droite;
    while (xCourant != limiteArriere && yCourant != limiteDroite) { // La case est dans la limite arrière droite
        if (Echiquier[xCourant][yCourant]->piece) { // La case n'est pas vide
            if (Echiquier[xCourant][yCourant]->piece->couleur != self->couleur) { // La case contient une pièce ennemie
                insertionCasesAtteignables(self, Echiquier[xCourant][yCourant]);
            }
            break; // On arrête la boucle à la rencontre d'une pièce
        } else { // La case est vide
            insertionCasesAtteignables(self, Echiquier[xCourant][yCourant]);
            xCourant -= avant;
            yCourant += droite;
        }
    }
    // Déplacement diagonale arrière gauche
    xCourant = self->x - avant;
    yCourant = self->y - droite;
    while (xCourant != limiteArriere && yCourant != limiteGauche) { // La case est dans la arrière gauche
        if (Echiquier[xCourant][yCourant]->piece) { // La case n'est pas vide
            if (Echiquier[xCourant][yCourant]->piece->couleur != self->couleur) { // La case contient une pièce ennemie
                insertionCasesAtteignables(self, Echiquier[xCourant][yCourant]);
            }
            break; // On arrête la boucle à la rencontre d'une pièce
        } else { // La case est vide
            insertionCasesAtteignables(self, Echiquier[xCourant][yCourant]);
            xCourant -= avant;
            yCourant -= droite;
        }
    }
    // Déplacement diagonale avant gauche
    xCourant = self->x + avant;
    yCourant = self->y - droite;
    while (xCourant != limiteAvant && yCourant != limiteGauche) { // La case est dans la limite avant
        if (Echiquier[xCourant][yCourant]->piece) { // La case n'est pas vide
            if (Echiquier[xCourant][yCourant]->piece->couleur != self->couleur) { // La case contient une pièce ennemie
                insertionCasesAtteignables(self, Echiquier[xCourant][yCourant]);
            }
            break; // On arrête la boucle à la rencontre d'une pièce
        } else { // La case est vide
            insertionCasesAtteignables(self, Echiquier[xCourant][yCourant]);
            xCourant += avant;
            yCourant -= droite;
        }
    }
}

void calculAtteignableReine(Case* Echiquier[8][8], Piece* self) {
    /*
    Met à jour le tableau des cases atteignables d'une reine.
    */

    calculAtteignableFou(Echiquier, self);
    calculAtteignableTour(Echiquier, self);
}

void calculAtteignableRoi(Case* Echiquier[8][8], Piece* self) {
    /*
    Met à jour le tableau des cases atteignables d'un roi.
    */

    int mouvements[8][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
    for (int i = 0; i < 8; i++) { // Pour chacun des mouvements
        int xCible = self->x + mouvements[i][0];
        int yCible = self->y + mouvements[i][1];

        if (xCible >= 0 && xCible < 8 && yCible >= 0 && yCible < 8) { // La case est dans les limites du plateau
            if ( (!Echiquier[xCible][yCible]->piece) || (Echiquier[xCible][yCible]->piece->couleur != self->couleur) ) {
                insertionCasesAtteignables(self, Echiquier[xCible][yCible]);
            }
        }
    }

    
    // Roque
    // Remarque : si les conditions du roque sont remplies, alors les cases intermédiaires sont déjà présentes dans le tableau des cases atteignables du roi, de même pour la tour
    int couleurAdverse = (self->couleur == BLANC) ? NOIR : BLANC;
    if ((!self->aPrecedemmentBouge) && (!Echiquier[self->x][self->y]->estAtteignableParJoueur[couleurAdverse])) { // Si le roi n'a jamais bougé et n'est pas en échec
        // Petit roque
        Piece *tourDroite = Echiquier[self->x][7]->piece;
        if ((tourDroite) && 
            (!tourDroite->aPrecedemmentBouge) && // Si elle n'a jamais bougée, c'est forcément notre tour
            // Si les cases entre le roi et la tour sont vides
            (!Echiquier[self->x][5]->piece) && 
            (!Echiquier[self->x][6]->piece) &&
            // Si les cases ou passe le roi ne sont pas menacées
            (!Echiquier[self->x][4]->estAtteignableParJoueur[couleurAdverse]) && 
            (!Echiquier[self->x][5]->estAtteignableParJoueur[couleurAdverse]) && 
            (!Echiquier[self->x][6]->estAtteignableParJoueur[couleurAdverse]))
        {
            insertionCasesAtteignables(self, Echiquier[self->x][6]);
        }
        // Grand roque
        Piece *tourGauche = Echiquier[self->x][0]->piece;
        if ((tourGauche) && 
            (!tourGauche->aPrecedemmentBouge) && // Si elle n'a jamais bougée, c'est forcément notre tour
            // Si les cases entre le roi et la tour sont vides
            (!Echiquier[self->x][1]->piece) && 
            (!Echiquier[self->x][2]->piece) && 
            (!Echiquier[self->x][3]->piece) &&
            // Si les cases ou passe le roi ne sont pas menacées
            (!Echiquier[self->x][2]->estAtteignableParJoueur[couleurAdverse]) && 
            (!Echiquier[self->x][3]->estAtteignableParJoueur[couleurAdverse]) &&
            (!Echiquier[self->x][4]->estAtteignableParJoueur[couleurAdverse]))
        {
            insertionCasesAtteignables(self, Echiquier[self->x][2]);
        }
    }
    
}