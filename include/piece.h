#ifndef PIECE_H
#define PIECE_H

#include <stdbool.h>

#include "echiquier.h"

#define NOIR 0
#define BLANC 1

#define COUVERTUREMAX 27

typedef enum {          
    ROI = 0,
    REINE = 9,
    FOU = 4,
    CAVALIER = 3,
    TOUR = 5,
    PION = 1,
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
    
    // État de la pièce
    bool estCapturee;
    bool estBloquee;
    bool aPrecedemmentBouge; // Pour autoriser la prise en passant et le roque

    // Gestion de la prise en passant
    bool vientDeFaireDoublePas; // Pour autoriser la prise en passant 
    bool vientDePrendreEnPassant; // Pour simuler la prise en passant

    // Gestion du roque
    bool vientDeRoquer; // Pour simuler le roque

    int longueurCasesAtteignables;

} Piece;

// Déclaration et suppresion de pièces
Piece* creationPiece(Role role, int couleur);

// Opérations sur les pièces
void insertionCasesAtteignables(Piece* pieceCourante, Case* caseAtteignable);
void suppressionCasesAtteignables(Piece* pieceCourante, Case* caseSupprimable);
void actualiseCasesAtteignablesParPiece(Piece* pieceCourante, Piece* piecePrecedente);
void actualiseCasesAtteignablesParJoueur(Case* Echiquier[8][8], Piece* Joueur[16]);
void actualiseExposeRoi(Case* Echiquier[8][8], Piece* joueurCourant[16], Piece* joueurAdverse[16]);

#endif // PIECE_H