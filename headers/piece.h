#ifndef PIECE_H
#define PIECE_H

#define NOIR 0
#define BLANC 1

#define COUVERTUREMAX 27

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

    void (*calculAtteignable)(Case* Echiquier[8][8], struct Piece* self);

    struct Case* casesAtteignables[COUVERTUREMAX];
    
    bool estSelectionnee;
    bool estCapturee;
    bool estBloquee;

    int longueurCasesAtteignables;

} Piece;

// Déclaration et suppresion de pièces
Piece* creationPiece(Role role, int couleur);

//TODO - Enlever joueurAdverse et roiAllie ?
// Opérations sur les pièces
void calculAtteignablePion(Case* Echiquier[8][8], Piece* self);
void calculAtteignableCavalier(Case* Echiquier[8][8], Piece* self);
void calculAtteignableTour(Case* Echiquier[8][8], Piece* self);
void calculAtteignableFou(Case* Echiquier[8][8], Piece* self);
void calculAtteignableReine(Case* Echiquier[8][8], Piece* self);
void calculAtteignableRoi(Case* Echiquier[8][8], Piece* self);

void insertionCasesAtteignables(Piece* pieceCourante, Case* caseAtteignable);
void actualiseCasesAtteignablesParPiece(Piece* pieceCourante, Piece* piecePrecedente);
void actualiseCasesAtteignablesParJoueur(Case* Echiquier[8][8], Piece* Joueur[16]);

// Opération sur les joueurs
void initialiseJoueur(Case* Echiquier[8][8], Piece* Joueur[16], int couleur);
void videJoueur(Piece* Joueur[16]); //TODO - Libère les pièces d'un joueur

#endif // PIECE_H