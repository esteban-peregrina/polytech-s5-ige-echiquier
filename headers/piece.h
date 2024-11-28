#ifndef PIECE_H
#define PIECE_H

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

typedef struct Piece {
    Role role;                   // Utile ?
    
    char forme;
    int couleur;                 // Couleur de la piece
    int x;
    int y;

    Piece* pieceSuivant; 
    Piece* piecePrecedent;        

    void (*calculAtteignable)(Piece* self);
    ListeCasesAtteignables* casesAtteignables;

} Piece;

typedef struct ListePiecesJoueur {
    Piece* tete;
} ListePiecesJoueur;

Piece* creationPiece(Role role, Case* caseCourante);
void destructionPiece(Piece* piece);

#endif // PIECE_H