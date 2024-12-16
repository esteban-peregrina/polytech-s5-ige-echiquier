#ifndef ECHIQUIER_H
#define ECHIQUIER_H

typedef enum {
    PIECES,            
    COUPS,
} Menu;

typedef struct Case {
    Piece* piece;

    int couleur;                        // Couleur de la case (? Necessaire ?)
    bool estSelectionne;
    bool estAtteignable;

    Case* caseAtteignableSuivante;               
    Case* caseAtteignablePrecedente;
} Case;

typedef struct ListeCasesAtteignables {
    Case* tete;
} ListeCasesAtteignables;

// Déclaration et suppression de cases
Case* creationCase();
void destructionCase(Case* Case);

// Opérations sur les cases
ListeCasesAtteignables* creationListeCasesAtteignables();
void destructionListeCasesAtteignables();
void insertionListeCasesAtteignables(Case* caseAtteignable, ListeCasesAtteignables* listeCasesAtteignables);
void supressionListeCasesAtteignables(Case* caseAtteignable, ListeCasesAtteignables* listeCasesAtteignables);

// Opérations sur le plateau
void initialisePlateau(Case Plateau[8][8]);
void actualiseCasesAtteignables(Piece* pieceCourante, Case Plateau[8][8]);
void affichePlateau(Case Plateau[8][8]);

#endif // ECHIQUIER_H