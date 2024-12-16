#include ".././headers/piece.h"
#include ".././headers/echiquier.h"


#include <stdlib.h>

void insertionListeCasesAtteignables(Case* caseAtteignable, ListeCasesAtteignables* listeCasesAtteignables) {
    /*
    Insère caseAtteignable en tete de listeCasesAtteignables
    */

    Case* temp = listeCasesAtteignables->tete;
    if (temp != NULL) { temp->caseAtteignablePrecedente == NULL; }
    listeCasesAtteignables->tete = caseAtteignable;
    caseAtteignable->caseAtteignableSuivante = temp;
    caseAtteignable->caseAtteignablePrecedente = NULL;
}

void partieEcheque() {
    /*
    Démarre une partie d'échec.
    */
    
    // Proposer de charger une sauvegarde
    // Proposer de jouer contre un joueur ou contre une IA

    

}