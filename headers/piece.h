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

    Piece* pieceSuivante; 
    Piece* piecePrecedente;        

    void (*calculAtteignable)(Piece* self, Case Plateau[8][8]);
    ListeCasesAtteignables* casesAtteignables;

} Piece;

typedef struct ListePiecesJoueur {
    Piece* tete;
} ListePiecesJoueur;

// Déclaration et suppresion de pièces
Piece* creationPiece(Role role, Case* caseCourante);
void destructionPiece(Piece* piece);

// Opération sur les joueurs
void initialiseJoueur(Piece Joueurs[2]);

#endif // PIECE_H