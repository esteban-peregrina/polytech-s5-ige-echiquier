#ifndef ECHIQUIER_H
#define ECHIQUIER_H

#define NOIR 0
#define BLANC 1

typedef enum {
    VIDE ,            
    ROI,
    REINE,
    FOU,
    CAVALIER,
    TOUR,
    PION,
} Piece;

typedef struct Case {
    Piece role;
    int couleur;            // Couleur de la piece
    bool estSelectionne;
    bool estAtteignable;
} Case;

#endif // ECHIQUIER_H