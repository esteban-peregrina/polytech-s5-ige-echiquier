#ifndef ECHIQUIER_H
#define ECHIQUIER_H

typedef enum {
    PIECES,            
    COUPS,
} Menu;

typedef struct Piece Piece; // Pour que le compilateur les reconnaissent

typedef struct Case {
    Piece* piece;
    bool estAtteignable;
    bool estSelectionnee;
} Case;

// Déclaration et suppression de cases
Case* creationCase(int couleur);

// Opérations sur l'echiquier
bool CaseExiste(int x, int y);
void initialiseEchiquier(Case* Echiquier[8][8]);
void videEchiquier(Case* Echiquier[8][8]);
void afficheEchiquier(Case* Echiquier[8][8]);

// Jeu
void partieEchec();

#endif // ECHIQUIER_H