#include <stdlib.h>
#include <stdbool.h>

#include ".././headers/echiquier.h"
#include ".././headers/piece.h"

bool CaseExiste(int x, int y){
    if(x<=8 && x>=1 && y<=8 && y >=1){                  
        return true;                          //Si les coordonées sont dans la limite du plateau alors la case existe
    }
    return false;
}

void calculAtteignablePion(Piece* self, Case Plateau[8][8]){
    int xPiece = self->x;
    int yPiece = self->y;
    int xPlateau = xPiece-1;
    int yPlateau = yPiece-1;

    if (self->couleur == BLANC){      
        if(xPiece<8 && Plateau[xPlateau+1][yPlateau].piece==NULL){                                                //Déplacements classique
            insertionListeCaseAtteignables(&Plateau[xPlateau+1][yPlateau], self->casesAtteignables);
        }
        if(xPiece == 2){
            if(Plateau[xPlateau+2][yPlateau].piece==NULL){                                                        //Avance de deux s'il est sur la ligne de départ
                insertionListeCaseAtteignables(&Plateau[xPlateau+2][yPlateau], self->casesAtteignables);
            }
        }
        if(xPiece<8 && yPiece<8 && Plateau[xPlateau+1][yPlateau+1].piece!=NULL && (Plateau[xPlateau+1][yPlateau+1].piece)->couleur != self->couleur){                                  //Possibilité de manger en diagonale
            insertionListeCaseAtteignables(&Plateau[xPlateau+1][yPlateau+1], self->casesAtteignables);
        }
        if(xPiece<8 && yPiece>1 && Plateau[xPlateau+1][yPlateau-1].piece!=NULL && (Plateau[xPlateau+1][yPlateau+1].piece)->couleur != self->couleur){
            insertionListeCaseAtteignables(&Plateau[xPlateau+1][yPlateau-1], self->casesAtteignables);
        }
    }else{
        if(xPiece >1 && Plateau[xPlateau-1][yPiece].piece==NULL){
            insertionListeCaseAtteignables(&Plateau[xPlateau-1][yPlateau], self->casesAtteignables);
        }
        if(xPiece == 7){
            if(Plateau[xPlateau-2][yPlateau].piece==NULL){
                insertionListeCaseAtteignables(&Plateau[xPlateau-2][yPlateau], self->casesAtteignables);
            }
        }
        if(xPiece>1 && yPiece<8 && Plateau[xPlateau-1][yPlateau+1].piece!=NULL && (Plateau[xPlateau+1][yPlateau+1].piece)->couleur != self->couleur){
            insertionListeCaseAtteignables(&Plateau[xPlateau-1][yPlateau+1], self->casesAtteignables);
        }
        if(xPiece>1 && yPiece>1 && Plateau[xPlateau+1][yPlateau-1].piece!=NULL && (Plateau[xPlateau+1][yPlateau+1].piece)->couleur != self->couleur){
            insertionListeCaseAtteignables(&Plateau[xPlateau-1][yPlateau-1], self->casesAtteignables);
        }
    }
}

void calculAtteignableTour(Piece* self, Case Plateau[8][8]){
    int xPiece = self->x;
    int yPiece = self->y;
    int xPlateau = xPiece-1;
    int yPlateau = yPiece-1;

    //Tour vers le haut (PDV des blancs)

    while(xPiece<8 && Plateau[xPlateau+1][yPlateau].piece==NULL){                                      
        insertionListeCaseAtteignables(&Plateau[xPlateau+1][yPlateau], self->casesAtteignables);
        xPiece++;
        xPlateau++;
    }
    if(xPiece != 8){
        if((Plateau[xPlateau+1][yPlateau].piece)->couleur != self->couleur){
            insertionListeCaseAtteignables(&Plateau[xPlateau+1][yPlateau], self->casesAtteignables);
        }
    }
    int xPiece = self->x;
    xPlateau = xPiece-1;

    //Tour vers le bas (PDV des blancs)

    while(xPiece>1 && Plateau[xPlateau-1][yPlateau].piece==NULL){
        insertionListeCaseAtteignables(&Plateau[xPlateau-1][yPlateau], self->casesAtteignables);
        xPiece--;
        xPlateau--;
    }
    if(xPiece != 1){
        if((Plateau[xPlateau-1][yPlateau].piece)->couleur != self->couleur){
            insertionListeCaseAtteignables(&Plateau[xPlateau+1][yPlateau], self->casesAtteignables);
        }
    }
    int xPiece = self->x;
    xPlateau = xPiece-1;

    //Tour vers la droite (PDV des blancs)

    while(yPiece<8 && Plateau[xPlateau][yPlateau+1].piece==NULL ){
        insertionListeCaseAtteignables(&Plateau[xPlateau][yPlateau+1], self->casesAtteignables);
        yPiece++;
        yPlateau++;
    }
    if(yPiece != 8){
        if((Plateau[xPlateau][yPlateau+1].piece)->couleur != self->couleur){
            insertionListeCaseAtteignables(&Plateau[xPlateau][yPlateau+1], self->casesAtteignables);
        }
    }
    int yPiece = self->y;
    yPlateau = yPiece-1;

    //Tour vers la gauche (PDV des blancs)

    while(yPiece>1 && Plateau[xPlateau][yPlateau-1].piece==NULL ){
        insertionListeCaseAtteignables(&Plateau[xPlateau][yPlateau-1], self->casesAtteignables);
        yPiece--;
        yPlateau--;
    }
    if(yPiece != 1){
        if((Plateau[xPlateau][yPlateau-1].piece)->couleur != self->couleur){
            insertionListeCaseAtteignables(&Plateau[xPlateau][yPlateau-1], self->casesAtteignables);
        }
    }
    int yPiece = self->y;
    yPlateau = yPiece-1;

}

void calculAtteignableCavalier(Piece* self, Case Plateau[8][8]){
    int xPiece = self->x;
    int yPiece = self->y;
    int xPlateau = xPiece-1;
    int yPlateau = yPiece-1;

    if(CaseExiste(xPiece+2,yPiece+1) && Plateau[xPlateau+2][yPlateau+1].piece == NULL){
        insertionListeCaseAtteignables(&Plateau[xPlateau+2][yPlateau+1], self->casesAtteignables);
    }
    if(CaseExiste(xPiece+2,yPiece-1) && Plateau[xPlateau+2][yPlateau-1].piece == NULL){
        insertionListeCaseAtteignables(&Plateau[xPlateau+2][yPlateau-1], self->casesAtteignables);
    }
    if(CaseExiste(xPiece+1,yPiece+2) && Plateau[xPlateau+1][yPlateau+2].piece == NULL){
        insertionListeCaseAtteignables(&Plateau[xPlateau+1][yPlateau+2], self->casesAtteignables);
    }
    if(CaseExiste(xPiece+1,yPiece-2) && Plateau[xPlateau+1][yPlateau-2].piece == NULL){
        insertionListeCaseAtteignables(&Plateau[xPlateau+1][yPlateau-2], self->casesAtteignables);
    }
    if(CaseExiste(xPiece-1,yPiece-2) && Plateau[xPlateau-1][yPlateau-2].piece == NULL){
        insertionListeCaseAtteignables(&Plateau[xPlateau-1][yPlateau-2], self->casesAtteignables);
    }
    if(CaseExiste(xPiece-1,yPiece+2) && Plateau[xPlateau-1][yPlateau+2].piece == NULL){
        insertionListeCaseAtteignables(&Plateau[xPlateau-1][yPlateau+2], self->casesAtteignables);
    }
    if(CaseExiste(xPiece-2,yPiece-1) && Plateau[xPlateau-2][yPlateau-1].piece == NULL){
        insertionListeCaseAtteignables(&Plateau[xPlateau-2][yPlateau-1], self->casesAtteignables);
    }
    if(CaseExiste(xPiece-2,yPiece+1) && Plateau[xPlateau-2][yPlateau+1].piece == NULL){
        insertionListeCaseAtteignables(&Plateau[xPlateau-2][yPlateau+1], self->casesAtteignables);
    }
}

void calculAtteignableFou(Piece* self, Case Plateau[8][8]){
    int xPiece = self->x;
    int yPiece = self->y;
    int xPlateau = xPiece-1;
    int yPlateau = yPiece-1;

    // fou diagonale haut-droit (PDV blancs)

    while(CaseExiste(xPiece + 1, yPiece + 1) && Plateau[xPlateau+1][yPlateau+1].piece == NULL){
        insertionListeCaseAtteignables(&Plateau[xPlateau+1][yPlateau+1], self->casesAtteignables);
        xPiece++;
        xPlateau++;
        yPiece++;
        yPlateau++;
    }
    if(CaseExiste(xPiece + 1, yPiece + 1)){
        if((Plateau[xPlateau+1][yPlateau+1].piece)->couleur != self->couleur){
            insertionListeCaseAtteignables(&Plateau[xPlateau+1][yPlateau+1], self->casesAtteignables);
        }
    }
    xPiece = self->x;
    xPlateau = xPiece-1;
    yPiece = self->y;
    yPlateau = yPiece-1;

    // fou diagonale haut-gauche(PDV blancs)

    while(CaseExiste(xPiece + 1, yPiece - 1) && Plateau[xPlateau+1][yPlateau-1].piece == NULL){
        insertionListeCaseAtteignables(&Plateau[xPlateau+1][yPlateau-1], self->casesAtteignables);
        xPiece++;
        xPlateau++;
        yPiece--;
        yPlateau--;
    }
    if(CaseExiste(xPiece + 1, yPiece - 1)){
        if((Plateau[xPlateau+1][yPlateau-1].piece)->couleur != self->couleur){
            insertionListeCaseAtteignables(&Plateau[xPlateau+1][yPlateau-1], self->casesAtteignables);
        }
    }
    xPiece = self->x;
    xPlateau = xPiece-1;
    yPiece = self->y;
    yPlateau = yPiece-1;

    // fou diagonale bas-droit (PDV blancs)

    while(CaseExiste(xPiece - 1, yPiece + 1) && Plateau[xPlateau-1][yPlateau+1].piece == NULL){
        insertionListeCaseAtteignables(&Plateau[xPlateau-1][yPlateau+1], self->casesAtteignables);
        xPiece--;
        xPlateau--;
        yPiece++;
        yPlateau++;
    }
    if(CaseExiste(xPiece - 1, yPiece + 1)){
        if((Plateau[xPlateau-1][yPlateau+1].piece)->couleur != self->couleur){
            insertionListeCaseAtteignables(&Plateau[xPlateau-1][yPlateau+1], self->casesAtteignables);
        }
    }
    xPiece = self->x;
    xPlateau = xPiece-1;
    yPiece = self->y;
    yPlateau = yPiece-1;

    // fou diagonale bas-gauche (PDV blancs)

    while(CaseExiste(xPiece - 1, yPiece - 1) && Plateau[xPlateau-1][yPlateau-1].piece == NULL){
        insertionListeCaseAtteignables(&Plateau[xPlateau-1][yPlateau-1], self->casesAtteignables);
        xPiece--;
        xPlateau--;
        yPiece--;
        yPlateau--;
    }
    if(CaseExiste(xPiece - 1, yPiece - 1)){
        if((Plateau[xPlateau-1][yPlateau-1].piece)->couleur != self->couleur){
            insertionListeCaseAtteignables(&Plateau[xPlateau-1][yPlateau-1], self->casesAtteignables);
        }
    }
}

void calculAtteignableReine(Piece* self, Case Plateau[8][8]){
    calculAtteignableFou(self,Plateau);
    calculAtteignableTour(self,Plateau);
}

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
                    if(caseCourante == &Plateau[x-1][y-1]){         //Si l'on tombe sur la case ou l'on souhaite aller
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