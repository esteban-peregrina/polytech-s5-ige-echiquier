#ifndef ECHIQUIER_H
#define ECHIQUIER_H

typedef enum {
    PIECES,            
    COUPS,
} Menu;

typedef struct Piece Piece; // Pour que le compilateur les reconnaissent

typedef struct Case {
    Piece* piece;

    int couleur;                        // Couleur de la case (? Necessaire ?)
    bool estSelectionne;
    bool estAtteignable;

    struct Case* caseAtteignableSuivante;               
    struct Case* caseAtteignablePrecedente;
} Case;

typedef struct ListeCasesAtteignables {
    Case* tete;
} ListeCasesAtteignables;

// Déclaration et suppression de cases
Case* creationCase(int couleur);
void destructionCase(Case* Case);

// Opérations sur les cases
ListeCasesAtteignables* creationListeCasesAtteignables();
void destructionListeCasesAtteignables(ListeCasesAtteignables* caseObsolete);
void insertionListeCasesAtteignables(Case* caseAtteignable, ListeCasesAtteignables* listeCasesAtteignables);
void supressionListeCasesAtteignables(Case* caseRetirable, ListeCasesAtteignables* listeCasesAtteignables);

// Opérations sur l'echiquier
bool CaseExiste(int x, int y);
void initialiseEchiquier(Case* Echiquier[8][8]);
void videEchiquier(Case* Echiquier[8][8]);
void afficheEchiquier(Case* Echiquier[8][8]);

void partieEchec();

#endif // ECHIQUIER_H