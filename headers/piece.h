#ifndef PIECE_H
#define PIECE_H

typedef struct Piece {
    Role role;
    char forme;
    int couleur;                 // Couleur de la piece

    int x;
    int y;

    Piece suivant;
    Piece precedent;        

    void (*calculAtteignable)(Piece* self);

    ListeCoupsAtteignables coupsAtteignables;
    
} Piece;

#endif // PIECE_H