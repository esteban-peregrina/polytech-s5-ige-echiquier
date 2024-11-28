#ifndef ECHIQUIER_H
#define ECHIQUIER_H

#define NOIR 0
#define BLANC 1

typedef enum {
    VIDE,            
    ROI,
    REINE,
    FOU,
    CAVALIER,
    TOUR,
    PION,
} Role;

typedef struct Case {
    Piece* piece;
    int couleur;            // Couleur de la case (? Necessaire ?)
    bool estSelectionne;
    bool estAtteignable;

    Case* suivant;
    Case* precedent;
} Case;

typedef struct ListeCoupsAtteignables {
    Case* Tete;
} ListeCoupsAtteignables;

#endif // ECHIQUIER_H