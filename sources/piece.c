#include ".././headers/echiquier.h"
#include ".././headers/piece.h"

#include <stdlib.h>
#include <stdbool.h>

bool CaseExiste(int x, int y){
    if(x<=8 && x>=1 && y<=8 && y >=1){                  
        return true;                          //Si les coordonées sont dans la limite du plateau alors la case existe
    }
    return false;
}

/* A corriger avec ma version locale !!!

void calculAtteignablePion(Piece* self, Case Plateau[8][8]){
    int xPiece = self->x;
    int yPiece = self->y;

    if (self->couleur == BLANC){                                                                              //Déplacements classique
        if(xPiece<8 && Plateau[xPiece+1][yPiece].piece==NULL){
            insertionListeCaseAtteignables(&Plateau[xPiece+1][yPiece], self->casesAtteignables);
        }
        if(xPiece == 2){
            if(Plateau[xPiece+2][yPiece].piece==NULL){                                                        //Avance de deux s'il est sur la ligne de départ
                insertionListeCaseAtteignables(&Plateau[xPiece+2][yPiece], self->casesAtteignables);
            }
        }
        if(xPiece<8 && yPiece<8 && Plateau[xPiece+1][yPiece+1].piece!=NULL){                                  //Possibilité de manger en diagonale
            insertionListeCaseAtteignables(&Plateau[xPiece+1][yPiece+1], self->casesAtteignables);
        }
        if(xPiece<8 && yPiece>1 && Plateau[xPiece+1][yPiece-1].piece!=NULL){
            insertionListeCaseAtteignables(&Plateau[xPiece+1][yPiece-1], self->casesAtteignables);
        }
    }else{
        if(xPiece >1 && Plateau[xPiece-1][yPiece].piece==NULL){
            insertionListeCaseAtteignables(&Plateau[xPiece-1][yPiece], self->casesAtteignables);
        }
        if(xPiece == 7){
            if(Plateau[xPiece-2][yPiece].piece==NULL){
                insertionListeCaseAtteignables(&Plateau[xPiece-2][yPiece], self->casesAtteignables);
            }
        }
        if(xPiece>1 && yPiece<8 && Plateau[xPiece-1][yPiece+1].piece!=NULL){
            insertionListeCaseAtteignables(&Plateau[xPiece-1][yPiece+1], self->casesAtteignables);
        }
        if(xPiece>1 && yPiece>1 && Plateau[xPiece+1][yPiece-1].piece!=NULL){
            insertionListeCaseAtteignables(&Plateau[xPiece-1][yPiece-1], self->casesAtteignables);

void calculAtteignableCavalier(Piece* self, Case Plateau[8][8]) {
    int xPiece = self->x;
    int yPiece = self->y;
}

*/

bool suisjeAtteignable(int x, int y, Case Plateau[8][8]){
    int i,j ;
 
    ListeCasesAtteignables* ListeCaseCourante;
    Piece* PieceCourante;
    Case* caseCourante;
    for(i=0;i<=7;i++){
        for(j=0;j<=7;j++){                                      //On parcourt tout l'échiquier
            if(Plateau[i][j].piece != NULL){                    //Si il y a une pièce sur la case
                Plateau[i][j].piece = PieceCourante;    
                //TODO actualise la liste (Reset + Calcul)
                ListeCaseCourante = PieceCourante->casesAtteignables;
                caseCourante = ListeCaseCourante->tete;
                while(caseCourante != NULL){                        //On parcourt la liste des cases atteignable associés a la piece
                    if(caseCourante == &Plateau[x-1][y-1]){         //Si on tombe sur la case ou on souhaite aller
                        return true;                                //Alors on sera atteignable
                    }
                }
            }
        }
    }
    return false;
}

void calculAtteignableRoi(Piece* self, Case Plateau[8][8]){
    int xPiece = self->x;
    int yPiece = self->y;
    int xPlateau = xPiece-1;
    int yPlateau = yPiece-1;
    if(CaseExiste(xPiece+1, yPiece) && !suisjeAtteignable(xPiece+1,yPiece, Plateau)){           //Si la case existe et n'est pas atteignable par une autre piece
        if(Plateau[xPlateau+1][yPlateau].piece==NULL || (Plateau[xPlateau+1][yPlateau].piece)->couleur != self->couleur){   //Si la case est libre ou occupé par un ennemi
            insertionListeCaseAtteignables(&Plateau[xPlateau+1][yPlateau], self->casesAtteignables);       //Alors on ajoute la case a la liste des cases atteignable
        }
    }
    if(CaseExiste(xPiece+1, yPiece-1) && !suisjeAtteignable(xPiece+1,yPiece-1, Plateau)){           //Si la case existe et n'est pas atteignable par une autre piece
        if(Plateau[xPlateau+1][yPlateau-1].piece==NULL || (Plateau[xPlateau+1][yPlateau-1].piece)->couleur != self->couleur){   //Si la case est libre ou occupé par un ennemi
            insertionListeCaseAtteignables(&Plateau[xPlateau+1][yPlateau-1], self->casesAtteignables);       //Alors on ajoute la case a la liste des cases atteignable
        }
    }
    if(CaseExiste(xPiece+1, yPiece+1) && !suisjeAtteignable(xPiece+1,yPiece+1, Plateau)){           //Si la case existe et n'est pas atteignable par une autre piece
        if(Plateau[xPlateau+1][yPlateau+1].piece==NULL || (Plateau[xPlateau+1][yPlateau+1].piece)->couleur != self->couleur){   //Si la case est libre ou occupé par un ennemi
            insertionListeCaseAtteignables(&Plateau[xPlateau+1][yPlateau+1], self->casesAtteignables);       //Alors on ajoute la case a la liste des cases atteignable
        }
    }
    if(CaseExiste(xPiece-1, yPiece) && !suisjeAtteignable(xPiece-1,yPiece, Plateau)){           //Si la case existe et n'est pas atteignable par une autre piece
        if(Plateau[xPlateau-1][yPlateau].piece==NULL || (Plateau[xPlateau-1][yPlateau].piece)->couleur != self->couleur){   //Si la case est libre ou occupé par un ennemi
            insertionListeCaseAtteignables(&Plateau[xPlateau-1][yPlateau], self->casesAtteignables);       //Alors on ajoute la case a la liste des cases atteignable
        }
    }
    if(CaseExiste(xPiece-1, yPiece+1) && !suisjeAtteignable(xPiece-1,yPiece+1, Plateau)){           //Si la case existe et n'est pas atteignable par une autre piece
        if(Plateau[xPlateau-1][yPlateau+1].piece==NULL || (Plateau[xPlateau-1][yPlateau+1].piece)->couleur != self->couleur){   //Si la case est libre ou occupé par un ennemi
            insertionListeCaseAtteignables(&Plateau[xPlateau-1][yPlateau+1], self->casesAtteignables);       //Alors on ajoute la case a la liste des cases atteignable
        }
    }
    if(CaseExiste(xPiece-1, yPiece-1) && !suisjeAtteignable(xPiece-1,yPiece-1, Plateau)){           //Si la case existe et n'est pas atteignable par une autre piece
        if(Plateau[xPlateau-1][yPlateau-1].piece==NULL || (Plateau[xPlateau-1][yPlateau-1].piece)->couleur != self->couleur){   //Si la case est libre ou occupé par un ennemi
            insertionListeCaseAtteignables(&Plateau[xPlateau-1][yPlateau-1], self->casesAtteignables);       //Alors on ajoute la case a la liste des cases atteignable
        }
    }
    if(CaseExiste(xPiece, yPiece+1) && !suisjeAtteignable(xPiece,yPiece+1, Plateau)){           //Si la case existe et n'est pas atteignable par une autre piece
        if(Plateau[xPlateau][yPlateau+1].piece==NULL || (Plateau[xPlateau][yPlateau+1].piece)->couleur != self->couleur){   //Si la case est libre ou occupé par un ennemi
            insertionListeCaseAtteignables(&Plateau[xPlateau][yPlateau+1], self->casesAtteignables);       //Alors on ajoute la case a la liste des cases atteignable
        }
    }
    if(CaseExiste(xPiece, yPiece-1) && !suisjeAtteignable(xPiece,yPiece-1, Plateau)){           //Si la case existe et n'est pas atteignable par une autre piece
        if(Plateau[xPlateau][yPlateau-1].piece==NULL || (Plateau[xPlateau][yPlateau-1].piece)->couleur != self->couleur){   //Si la case est libre ou occupé par un ennemi
            insertionListeCaseAtteignables(&Plateau[xPlateau][yPlateau-1], self->casesAtteignables);       //Alors on ajoute la case a la liste des cases atteignable
        }
    }
    
}