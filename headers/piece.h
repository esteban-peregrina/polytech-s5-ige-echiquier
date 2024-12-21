#ifndef PIECE_H
#define PIECE_H

#define NOIR 0
#define BLANC 1

typedef enum {          
    ROI,
    REINE,
    FOU,
    CAVALIER,
    TOUR,
    PION,
} Role;

typedef struct Case Case; // Pour que le compilateur les reconnaissent

typedef struct Piece {
    Role role;                   // Évite d'utiliser l'attribut forme dans les conditions 

    char* forme;                 
    int couleur;                 // Couleur de la piece
    int x;
    int y;      

    void (*calculAtteignable)(struct Piece* self, Case* Plateau[8][8], struct Piece* Joueur[16]);

    struct Case* casesAtteignables[64];
    
    bool estSelectionnee;

} Piece;

// Déclaration et suppresion de pièces
Piece* creationPiece(Role role, int couleur);
void destructionPiece(Piece* piece);

// Opérations sur les pièces
void calculAtteignablePion(Piece* self, Case* Echiquier[8][8], Piece* Joueurs[2]);
void calculAtteignableTour(Piece* self, Case* Echiquier[8][8], Piece* Joueurs[2]);
void calculAtteignableCavalier(Piece* self, Case* Echiquier[8][8], Piece* Joueurs[2]);
void calculAtteignableFou(Piece* self, Case* Echiquier[8][8], Piece* Joueurs[2]);
void calculAtteignableReine(Piece* self, Case* Echiquier[8][8], Piece* Joueurs[2]);
void calculAtteignableRoi(Piece* self, Case* Echiquier[8][8], Piece* Joueurs[2]);
void actualiseCasesAtteignables(Piece* piecePrecedente, Piece* pieceCourante, Case* Echiquier[8][8], Piece* Joueurs[2]); // Vide ListeCasesAtteignables de piecePrecedente et rempli celle de pieceCourante

// Opérations sur le Roi
bool suisjeAtteignable(int xCible, int yCible, Case* Echiquier[8][8]);
bool leaveRoi(Piece* self, int xCible, int yCible, Piece* Joueurs[2], Case* Echiquier[8][8]);

// Opération sur les joueurs
void insertionPieceJoueur(Piece* pieceAjoutable, Piece* Joueurs[2]);
void supressionPieceJoueur(Piece* pieceRetirable, Piece* Joueurs[2]);
void initialiseJoueur(Piece* Joueurs[2], Case* Echiquier[8][8]);

#endif // PIECE_H