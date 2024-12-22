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

    void (*calculAtteignable)(Case* Echiquier[8][8], struct Piece* Roi, struct Piece* self);

    struct Case* casesAtteignables[64];
    
    bool estSelectionnee;
    bool estCapturee;

} Piece;

// Déclaration et suppresion de pièces
Piece* creationPiece(Role role, int couleur);

// Opérations sur les pièces
void calculAtteignablePion(Case* Echiquier[8][8], Piece* Roi, Piece* self);
void calculAtteignableTour(Case* Echiquier[8][8], Piece* Roi, Piece* self);
void calculAtteignableCavalier(Case* Echiquier[8][8], Piece* Roi, Piece* self);
void calculAtteignableFou(Case* Echiquier[8][8], Piece* Roi, Piece* self);
void calculAtteignableReine(Case* Echiquier[8][8], Piece* Roi, Piece* self);
void calculAtteignableRoi(Case* Echiquier[8][8], Piece* Roi, Piece* self);

void insertionCasesAtteignables(Piece* pieceCourante, Case* caseAtteignable);
void actualiseCasesAtteignables(Case* Echiquier[8][8], Piece* Roi, Piece* pieceActualisable);

// Opérations sur le Roi
bool suisjeAtteignable(int xCible, int yCible, Case* Echiquier[8][8]);
bool leaveRoi(Piece* self, int xCible, int yCible, Piece* Joueurs[2], Case* Echiquier[8][8]);

// Opération sur les joueurs
void initialiseJoueur(Case* Echiquier[8][8], Piece* Joueur[16], int couleur);

#endif // PIECE_H