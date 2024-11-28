#ifndef ECHIQUIER_H
#define ECHIQUIER_H

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

Case* creationCase();
void destructionCase(Case* Case);

ListeCasesAtteignables* creationListeCasesAtteignables();
void destructionListeCasesAtteignables();
void insertionListeCasesAtteignables(Case* caseAtteignable, ListeCasesAtteignables* listeCasesAtteignables);
void supressionListeCasesAtteignables(Case* caseAtteignable, ListeCasesAtteignables* listeCasesAtteignables);

#endif // ECHIQUIER_H