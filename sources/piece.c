#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include ".././headers/echiquier.h"
#include ".././headers/piece.h"

Piece* creationPiece(Role role, int couleur) {
    /*
    Renvoie l'adresse de la Piece créée, initialisée avec le role specifié.
    */

    Piece* pieceCree = NULL;
    pieceCree = malloc(sizeof(Piece));
    if (pieceCree == NULL) { exit(EXIT_FAILURE); }

    pieceCree->role = role;

    if (role == ROI) {
        pieceCree->forme = "♚";
        pieceCree->calculAtteignable = calculAtteignableRoi;
    } else if (role == REINE) {
        pieceCree->forme = "♛";
        pieceCree->calculAtteignable = calculAtteignableReine;
    } else if (role == FOU) {
        pieceCree->forme = "♝";
        pieceCree->calculAtteignable = calculAtteignableFou;
    } else if (role == CAVALIER) {
        pieceCree->forme = "♞";
        pieceCree->calculAtteignable = calculAtteignableCavalier;
    } else if (role == TOUR) {
        pieceCree->forme = "♜";
        pieceCree->calculAtteignable = calculAtteignableTour;
    } else if (role == PION) {
        pieceCree->forme = "♟";
        pieceCree->calculAtteignable = calculAtteignablePion;
    }
    pieceCree->couleur = couleur;
    pieceCree->x = 0;
    pieceCree->y = 0;

    pieceCree->pieceSuivante = NULL;
    pieceCree->piecePrecedente = NULL;

    pieceCree->casesAtteignables = creationListeCasesAtteignables();

    return pieceCree;
}


void destructionPiece(Piece* piece) {
    /*
    Libère la piece pointée.  
    */

    if (piece == NULL) { exit(EXIT_FAILURE); } // On ne devrait pas passer de Piece vide à cette fonction
    destructionListeCasesAtteignables(piece->casesAtteignables);
    free(piece);
}

bool CaseExiste(int x, int y){
    if(x<=8 && x>=1 && y<=8 && y >=1){                  
        return true;                          //Si les coordonées sont dans la limite du Echiquier alors la case existe
    }
    return false;
}

void calculAtteignablePion(Piece* self, Case* Echiquier[8][8], Piece* Joueurs[2]){
    int xPiece = self->x;
    int yPiece = self->y;
    int xEchiquier = xPiece-1;
    int yEchiquier = yPiece-1;

    if (self->couleur == BLANC){      
        if(xPiece<8 && Echiquier[xEchiquier+1][yEchiquier]->piece==NULL && !leaveRoi(self,xPiece+1,yPiece,Joueurs,Echiquier)){                                                //Déplacements classique
            insertionListeCasesAtteignables(Echiquier[xEchiquier+1][yEchiquier], self->casesAtteignables);
        }
        if(xPiece == 2){
            if(Echiquier[xEchiquier+2][yEchiquier]->piece==NULL && !leaveRoi(self,xPiece+2,yPiece,Joueurs,Echiquier)){                                                        //Avance de deux s'il est sur la ligne de départ
                insertionListeCasesAtteignables(Echiquier[xEchiquier+2][yEchiquier], self->casesAtteignables);
            }
        }
        if(xPiece<8 && yPiece<8 && Echiquier[xEchiquier+1][yEchiquier+1]->piece!=NULL && (Echiquier[xEchiquier+1][yEchiquier+1]->piece)->couleur != self->couleur && !leaveRoi(self,xPiece+1,yPiece+1,Joueurs,Echiquier)){                                  //Possibilité de manger en diagonale
            insertionListeCasesAtteignables(Echiquier[xEchiquier+1][yEchiquier+1], self->casesAtteignables);
        }
        if(xPiece<8 && yPiece>1 && Echiquier[xEchiquier+1][yEchiquier-1]->piece!=NULL && (Echiquier[xEchiquier+1][yEchiquier+1]->piece)->couleur != self->couleur && !leaveRoi(self,xPiece+1,yPiece+1,Joueurs,Echiquier)){
            insertionListeCasesAtteignables(Echiquier[xEchiquier+1][yEchiquier-1], self->casesAtteignables);
        }
    }else{
        if(xPiece >1 && Echiquier[xEchiquier-1][yPiece]->piece==NULL && !leaveRoi(self,xPiece-1,yPiece,Joueurs,Echiquier)){
            insertionListeCasesAtteignables(Echiquier[xEchiquier-1][yEchiquier], self->casesAtteignables);
        }
        if(xPiece == 7){
            if(Echiquier[xEchiquier-2][yEchiquier]->piece==NULL && !leaveRoi(self,xPiece-2,yPiece,Joueurs,Echiquier)){
                insertionListeCasesAtteignables(Echiquier[xEchiquier-2][yEchiquier], self->casesAtteignables);
            }
        }
        if(xPiece>1 && yPiece<8 && Echiquier[xEchiquier-1][yEchiquier+1]->piece!=NULL && (Echiquier[xEchiquier-1][yEchiquier+1]->piece)->couleur != self->couleur && !leaveRoi(self,xPiece-1,yPiece+1,Joueurs,Echiquier)){
            insertionListeCasesAtteignables(Echiquier[xEchiquier-1][yEchiquier+1], self->casesAtteignables);
        }
        if(xPiece>1 && yPiece>1 && Echiquier[xEchiquier-1][yEchiquier-1]->piece!=NULL && (Echiquier[xEchiquier-1][yEchiquier-1]->piece)->couleur != self->couleur && !leaveRoi(self,xPiece-1,yPiece-1,Joueurs,Echiquier)){
            insertionListeCasesAtteignables(Echiquier[xEchiquier-1][yEchiquier-1], self->casesAtteignables);
        }
    }
}

void calculAtteignableTour(Piece* self, Case* Echiquier[8][8], Piece* Joueurs[2]){
    int xPiece = self->x;
    int yPiece = self->y;
    int xEchiquier = xPiece-1;
    int yEchiquier = yPiece-1;

    //Tour vers le haut (PDV des blancs)

    while(xPiece<8 && Echiquier[xEchiquier+1][yEchiquier]->piece==NULL && !leaveRoi(self,xPiece+1,yPiece,Joueurs,Echiquier)){                                      
        insertionListeCasesAtteignables(Echiquier[xEchiquier+1][yEchiquier], self->casesAtteignables);
        xPiece++;
        xEchiquier++;
    }
    if(Echiquier[xEchiquier+1][yEchiquier]->piece!=NULL){
        if((Echiquier[xEchiquier+1][yEchiquier]->piece)->couleur != self->couleur && !leaveRoi(self,xPiece+1,yPiece,Joueurs,Echiquier)){
            insertionListeCasesAtteignables(Echiquier[xEchiquier+1][yEchiquier], self->casesAtteignables);
        }
    }
    xPiece = self->x;
    xEchiquier = xPiece-1;

    //Tour vers le bas (PDV des blancs)

    while(xPiece>1 && Echiquier[xEchiquier-1][yEchiquier]->piece==NULL && !leaveRoi(self,xPiece-1,yPiece,Joueurs,Echiquier)){
        insertionListeCasesAtteignables(Echiquier[xEchiquier-1][yEchiquier], self->casesAtteignables);
        xPiece--;
        xEchiquier--;
    }
    if(Echiquier[xEchiquier-1][yEchiquier]->piece!=NULL){
        if((Echiquier[xEchiquier-1][yEchiquier]->piece)->couleur != self->couleur){
            insertionListeCasesAtteignables(Echiquier[xEchiquier+1][yEchiquier], self->casesAtteignables);
        }
    }
    xPiece = self->x;
    xEchiquier = xPiece-1;

    //Tour vers la droite (PDV des blancs)

    while(yPiece<8 && Echiquier[xEchiquier][yEchiquier+1]->piece==NULL && !leaveRoi(self,xPiece,yPiece+1,Joueurs,Echiquier)){
        insertionListeCasesAtteignables(Echiquier[xEchiquier][yEchiquier+1], self->casesAtteignables);
        yPiece++;
        yEchiquier++;
    }
    if(Echiquier[xEchiquier][yEchiquier+1]->piece!=NULL){
        if((Echiquier[xEchiquier][yEchiquier+1]->piece)->couleur != self->couleur){
            insertionListeCasesAtteignables(Echiquier[xEchiquier][yEchiquier+1], self->casesAtteignables);
        }
    }
    yPiece = self->y;
    yEchiquier = yPiece-1;

    //Tour vers la gauche (PDV des blancs)

    while(yPiece>1 && Echiquier[xEchiquier][yEchiquier-1]->piece==NULL && !leaveRoi(self,xPiece,yPiece-1,Joueurs,Echiquier)){
        insertionListeCasesAtteignables(Echiquier[xEchiquier][yEchiquier-1], self->casesAtteignables);
        yPiece--;
        yEchiquier--;
    }
    if(Echiquier[xEchiquier][yEchiquier-1]->piece!=NULL){
        if((Echiquier[xEchiquier][yEchiquier-1]->piece)->couleur != self->couleur){
            insertionListeCasesAtteignables(Echiquier[xEchiquier][yEchiquier-1], self->casesAtteignables);
        }
    }
    yPiece = self->y;
    yEchiquier = yPiece-1;

}

void calculAtteignableCavalier(Piece* self, Case* Echiquier[8][8], Piece* Joueurs[2]){
    int xPiece = self->x;
    int yPiece = self->y;
    int xEchiquier = xPiece-1;
    int yEchiquier = yPiece-1;

    if(CaseExiste(xPiece+2,yPiece+1) && Echiquier[xEchiquier+2][yEchiquier+1]->piece == NULL && !leaveRoi(self,xPiece+2,yPiece+1,Joueurs,Echiquier)){
        insertionListeCasesAtteignables(Echiquier[xEchiquier+2][yEchiquier+1], self->casesAtteignables);
    }
    if(CaseExiste(xPiece+2,yPiece-1) && Echiquier[xEchiquier+2][yEchiquier-1]->piece == NULL && !leaveRoi(self,xPiece+2,yPiece-1,Joueurs,Echiquier)){
        insertionListeCasesAtteignables(Echiquier[xEchiquier+2][yEchiquier-1], self->casesAtteignables);
    }
    if(CaseExiste(xPiece+1,yPiece+2) && Echiquier[xEchiquier+1][yEchiquier+2]->piece == NULL && !leaveRoi(self,xPiece+1,yPiece+2,Joueurs,Echiquier)){
        insertionListeCasesAtteignables(Echiquier[xEchiquier+1][yEchiquier+2], self->casesAtteignables);
    }
    if(CaseExiste(xPiece+1,yPiece-2) && Echiquier[xEchiquier+1][yEchiquier-2]->piece == NULL && !leaveRoi(self,xPiece+1,yPiece-2,Joueurs,Echiquier)){
        insertionListeCasesAtteignables(Echiquier[xEchiquier+1][yEchiquier-2], self->casesAtteignables);
    }
    if(CaseExiste(xPiece-1,yPiece-2) && Echiquier[xEchiquier-1][yEchiquier-2]->piece == NULL && !leaveRoi(self,xPiece-1,yPiece-2,Joueurs,Echiquier)){
        insertionListeCasesAtteignables(Echiquier[xEchiquier-1][yEchiquier-2], self->casesAtteignables);
    }
    if(CaseExiste(xPiece-1,yPiece+2) && Echiquier[xEchiquier-1][yEchiquier+2]->piece == NULL && !leaveRoi(self,xPiece-1,yPiece+2,Joueurs,Echiquier)){
        insertionListeCasesAtteignables(Echiquier[xEchiquier-1][yEchiquier+2], self->casesAtteignables);
    }
    if(CaseExiste(xPiece-2,yPiece-1) && Echiquier[xEchiquier-2][yEchiquier-1]->piece == NULL && !leaveRoi(self,xPiece-2,yPiece-1,Joueurs,Echiquier)){
        insertionListeCasesAtteignables(Echiquier[xEchiquier-2][yEchiquier-1], self->casesAtteignables);
    }
    if(CaseExiste(xPiece-2,yPiece+1) && Echiquier[xEchiquier-2][yEchiquier+1]->piece == NULL && !leaveRoi(self,xPiece-2,yPiece+1,Joueurs,Echiquier)){
        insertionListeCasesAtteignables(Echiquier[xEchiquier-2][yEchiquier+1], self->casesAtteignables);
    }
}

void calculAtteignableFou(Piece* self, Case* Echiquier[8][8], Piece* Joueurs[2]){
    int xPiece = self->x;
    int yPiece = self->y;
    int xEchiquier = xPiece-1;
    int yEchiquier = yPiece-1;

    // fou diagonale haut-droit (PDV blancs)

    while(CaseExiste(xPiece + 1, yPiece + 1) && Echiquier[xEchiquier+1][yEchiquier+1]->piece == NULL && !leaveRoi(self,xPiece+1,yPiece+1,Joueurs,Echiquier)){
        insertionListeCasesAtteignables(Echiquier[xEchiquier+1][yEchiquier+1], self->casesAtteignables);
        xPiece++;
        xEchiquier++;
        yPiece++;
        yEchiquier++;
    }
    if(Echiquier[xEchiquier+1][yEchiquier+1]->piece != NULL){
        if((Echiquier[xEchiquier+1][yEchiquier+1]->piece)->couleur != self->couleur){
            insertionListeCasesAtteignables(Echiquier[xEchiquier+1][yEchiquier+1], self->casesAtteignables);
        }
    }
    xPiece = self->x;
    xEchiquier = xPiece-1;
    yPiece = self->y;
    yEchiquier = yPiece-1;

    // fou diagonale haut-gauche(PDV blancs)

    while(CaseExiste(xPiece + 1, yPiece - 1) && Echiquier[xEchiquier+1][yEchiquier-1]->piece == NULL && !leaveRoi(self,xPiece+1,yPiece-1,Joueurs,Echiquier)){
        insertionListeCasesAtteignables(Echiquier[xEchiquier+1][yEchiquier-1], self->casesAtteignables);
        xPiece++;
        xEchiquier++;
        yPiece--;
        yEchiquier--;
    }
    if(Echiquier[xEchiquier+1][yEchiquier-1]->piece != NULL){
        if((Echiquier[xEchiquier+1][yEchiquier-1]->piece)->couleur != self->couleur){
            insertionListeCasesAtteignables(Echiquier[xEchiquier+1][yEchiquier-1], self->casesAtteignables);
        }
    }
    xPiece = self->x;
    xEchiquier = xPiece-1;
    yPiece = self->y;
    yEchiquier = yPiece-1;

    // fou diagonale bas-droit (PDV blancs)

    while(CaseExiste(xPiece - 1, yPiece + 1) && Echiquier[xEchiquier-1][yEchiquier+1]->piece == NULL && !leaveRoi(self,xPiece-1,yPiece+1,Joueurs,Echiquier)){
        insertionListeCasesAtteignables(Echiquier[xEchiquier-1][yEchiquier+1], self->casesAtteignables);
        xPiece--;
        xEchiquier--;
        yPiece++;
        yEchiquier++;
    }
    if(Echiquier[xEchiquier-1][yEchiquier+1]->piece != NULL){
        if((Echiquier[xEchiquier-1][yEchiquier+1]->piece)->couleur != self->couleur){
            insertionListeCasesAtteignables(Echiquier[xEchiquier-1][yEchiquier+1], self->casesAtteignables);
        }
    }
    xPiece = self->x;
    xEchiquier = xPiece-1;
    yPiece = self->y;
    yEchiquier = yPiece-1;

    // fou diagonale bas-gauche (PDV blancs)

    while(CaseExiste(xPiece - 1, yPiece - 1) && Echiquier[xEchiquier-1][yEchiquier-1]->piece == NULL && !leaveRoi(self,xPiece-1,yPiece-1,Joueurs,Echiquier)){
        insertionListeCasesAtteignables(Echiquier[xEchiquier-1][yEchiquier-1], self->casesAtteignables);
        xPiece--;
        xEchiquier--;
        yPiece--;
        yEchiquier--;
    }
    if(Echiquier[xEchiquier-1][yEchiquier-1]->piece == NULL){
        if((Echiquier[xEchiquier-1][yEchiquier-1]->piece)->couleur != self->couleur){
            insertionListeCasesAtteignables(Echiquier[xEchiquier-1][yEchiquier-1], self->casesAtteignables);
        }
    }
}

void calculAtteignableReine(Piece* self, Case* Echiquier[8][8], Piece* Joueurs[2]){
    calculAtteignableFou(self,Echiquier, Joueurs);
    calculAtteignableTour(self,Echiquier, Joueurs);
}

bool suisjeAtteignable(int x, int y, Case* Echiquier[8][8]){
    int i,j ;
 
    ListeCasesAtteignables* ListeCaseCourante;
    Piece* PieceCourante;
    Case* caseCourante;
    for(i=0;i<=7;i++){
        for(j=0;j<=7;j++){                                      //On parcourt tout l'échiquier
            if(Echiquier[i][j]->piece != NULL){                    //Si il y a une pièce sur la case
                PieceCourante = Echiquier[i][j]->piece;     
                //TODO actualise la liste (Reset + Calcul)
                ListeCaseCourante = PieceCourante->casesAtteignables;
                caseCourante = ListeCaseCourante->tete;
                while(caseCourante != NULL){                        //On parcourt la liste des cases atteignable associés a la piece
                    if(caseCourante == Echiquier[x-1][y-1]){         //Si l'on tombe sur la case ou l'on souhaite aller
                        return true;                                //Alors on sera atteignable
                    }
                }
            }
        }
    }
    return false;
}

void calculAtteignableRoi(Piece* self, Case* Echiquier[8][8], Piece* Joueurs[2]){
    int xPiece = self->x;
    int yPiece = self->y;
    int xEchiquier = xPiece-1;
    int yEchiquier = yPiece-1;
    if(CaseExiste(xPiece+1, yPiece) && !suisjeAtteignable(xPiece+1,yPiece, Echiquier)){                                           //Si la case existe et n'est pas atteignable par une autre piece
        if(Echiquier[xEchiquier+1][yEchiquier]->piece==NULL || (Echiquier[xEchiquier+1][yEchiquier]->piece)->couleur != self->couleur){       //Si la case est libre ou occupé par un ennemi
            insertionListeCasesAtteignables(Echiquier[xEchiquier+1][yEchiquier], self->casesAtteignables);                           //Alors on ajoute la case a la liste des cases atteignable
        }
    }
    if(CaseExiste(xPiece+1, yPiece-1) && !suisjeAtteignable(xPiece+1,yPiece-1, Echiquier)){                                       //Si la case existe et n'est pas atteignable par une autre piece
        if(Echiquier[xEchiquier+1][yEchiquier-1]->piece==NULL || (Echiquier[xEchiquier+1][yEchiquier-1]->piece)->couleur != self->couleur){   //Si la case est libre ou occupé par un ennemi
            insertionListeCasesAtteignables(Echiquier[xEchiquier+1][yEchiquier-1], self->casesAtteignables);                         //Alors on ajoute la case a la liste des cases atteignable
        }
    }
    if(CaseExiste(xPiece+1, yPiece+1) && !suisjeAtteignable(xPiece+1,yPiece+1, Echiquier)){                                       //Si la case existe et n'est pas atteignable par une autre piece
        if(Echiquier[xEchiquier+1][yEchiquier+1]->piece==NULL || (Echiquier[xEchiquier+1][yEchiquier+1]->piece)->couleur != self->couleur){   //Si la case est libre ou occupé par un ennemi
            insertionListeCasesAtteignables(Echiquier[xEchiquier+1][yEchiquier+1], self->casesAtteignables);                         //Alors on ajoute la case a la liste des cases atteignable
        }
    }
    if(CaseExiste(xPiece-1, yPiece) && !suisjeAtteignable(xPiece-1,yPiece, Echiquier)){                                           //Si la case existe et n'est pas atteignable par une autre piece
        if(Echiquier[xEchiquier-1][yEchiquier]->piece==NULL || (Echiquier[xEchiquier-1][yEchiquier]->piece)->couleur != self->couleur){       //Si la case est libre ou occupé par un ennemi
            insertionListeCasesAtteignables(Echiquier[xEchiquier-1][yEchiquier], self->casesAtteignables);                           //Alors on ajoute la case a la liste des cases atteignable
        }
    }
    if(CaseExiste(xPiece-1, yPiece+1) && !suisjeAtteignable(xPiece-1,yPiece+1, Echiquier)){                                       //Si la case existe et n'est pas atteignable par une autre piece
        if(Echiquier[xEchiquier-1][yEchiquier+1]->piece==NULL || (Echiquier[xEchiquier-1][yEchiquier+1]->piece)->couleur != self->couleur){   //Si la case est libre ou occupé par un ennemi
            insertionListeCasesAtteignables(Echiquier[xEchiquier-1][yEchiquier+1], self->casesAtteignables);                         //Alors on ajoute la case a la liste des cases atteignable
        }
    }
    if(CaseExiste(xPiece-1, yPiece-1) && !suisjeAtteignable(xPiece-1,yPiece-1, Echiquier)){                                       //Si la case existe et n'est pas atteignable par une autre piece
        if(Echiquier[xEchiquier-1][yEchiquier-1]->piece==NULL || (Echiquier[xEchiquier-1][yEchiquier-1]->piece)->couleur != self->couleur){   //Si la case est libre ou occupé par un ennemi
            insertionListeCasesAtteignables(Echiquier[xEchiquier-1][yEchiquier-1], self->casesAtteignables);                         //Alors on ajoute la case a la liste des cases atteignable
        }
    }
    if(CaseExiste(xPiece, yPiece+1) && !suisjeAtteignable(xPiece,yPiece+1, Echiquier)){                                           //Si la case existe et n'est pas atteignable par une autre piece
        if(Echiquier[xEchiquier][yEchiquier+1]->piece==NULL || (Echiquier[xEchiquier][yEchiquier+1]->piece)->couleur != self->couleur){       //Si la case est libre ou occupé par un ennemi
            insertionListeCasesAtteignables(Echiquier[xEchiquier][yEchiquier+1], self->casesAtteignables);                           //Alors on ajoute la case a la liste des cases atteignable
        }
    }
    if(CaseExiste(xPiece, yPiece-1) && !suisjeAtteignable(xPiece,yPiece-1, Echiquier)){                                           //Si la case existe et n'est pas atteignable par une autre piece
        if(Echiquier[xEchiquier][yEchiquier-1]->piece==NULL || (Echiquier[xEchiquier][yEchiquier-1]->piece)->couleur != self->couleur){       //Si la case est libre ou occupé par un ennemi
            insertionListeCasesAtteignables(Echiquier[xEchiquier][yEchiquier-1], self->casesAtteignables);                           //Alors on ajoute la case a la liste des cases atteignable
        }
    }
    
}

bool leaveRoi(Piece* self, int x, int y, Piece* Joueurs[2], Case* Echiquier[8][8]){      //(x,y) : case ou je souhaite me rendre
    Piece* roi = (self->couleur == BLANC) ? Joueurs[BLANC] : Joueurs[NOIR];  
    Piece* StockPiece;
    bool res;
    if(Echiquier[x-1][y-1]->piece == NULL){                                                //Si la case ciblée est vide :
        Echiquier[x-1][y-1]->piece = Echiquier[self->x-1][self->y-1]->piece;               //Deplace la piece 
        Echiquier[self->x-1][self->y-1]->piece = NULL;  
        res = suisjeAtteignable(roi->x, roi->y, Echiquier);                          //Verifie si le roi est atteignable apres le deplacement
        Echiquier[self->x-1][self->y-1]->piece = Echiquier[x-1][y-1]->piece;               //Remet la Piece au bon endroit
        Echiquier[x-1][y-1]->piece = NULL;
        return res;                                                                        //Return true si le déplacement decouvre notre roi et false sinon
    }                   
    else{                                                                                  //Sinon (case occupé par un ennemi) :
        StockPiece = Echiquier[x-1][y-1]->piece;                                           //Met de cote la piece présente sur la case ciblée
        Echiquier[x-1][y-1]->piece = Echiquier[self->x-1][self->y-1]->piece;               //Deplace la piece
        Echiquier[self->x-1][self->y-1]->piece = NULL;
        res = suisjeAtteignable(roi->x, roi->y, Echiquier);                          //Verifie si le roi est atteignable apres le deplacement
        Echiquier[self->x-1][self->y-1]->piece = Echiquier[x-1][y-1]->piece;
        Echiquier[x-1][y-1]->piece = StockPiece;
        StockPiece = NULL;
        return res;                                                                        //Return true si le déplacement decouvre notre roi et false sinon
    }                                               
}

void actualiseCasesAtteignables(Piece* piecePrecedente, Piece* pieceCourante, Case* Echiquier[8][8], Piece* Joueurs[2]) {
    /*
    Vide la piecePrecedente->casesAtteignables et rempli pieceSuivante->casesAtteignables.
    */

    if (piecePrecedente == NULL || pieceCourante == NULL) { exit(EXIT_FAILURE); }
    while(piecePrecedente->casesAtteignables->tete != NULL) {
        supressionListeCasesAtteignables(piecePrecedente->casesAtteignables->tete, piecePrecedente->casesAtteignables);
    }
    pieceCourante->calculAtteignable(pieceCourante, Echiquier, Joueurs);
}

void insertionPieceJoueur(Piece* pieceAjoutable, Piece* Joueurs[2]) {
    /*
    Insère pieceAjoutable à la main du joueur de sa couleur. Les rois et reines sont déjà présents.
    */

    int couleur = pieceAjoutable->couleur;
    Piece* temp = Joueurs[couleur]->pieceSuivante; //La piece juste après le roi
    Joueurs[couleur]->pieceSuivante = pieceAjoutable; // On connecte le roi à la pice
    pieceAjoutable->piecePrecedente = Joueurs[couleur]; // On connecte la piece au roi
    pieceAjoutable->pieceSuivante = temp; // On connecte la piece à celle précédement après le roi
    temp->piecePrecedente = pieceAjoutable; // On connecte celle précédemment après le roi à la pièce
}

// TODO - SuppressionPieceJoueur()

void initialiseJoueur(Piece* Joueurs[2], Case* Echiquier[8][8]) {
    /*
    Initialise les pièces de chacun des joueurs.
    */
    for (int couleur = NOIR; couleur < BLANC + 1; couleur++) {
        // Roi
        Echiquier[3][7]->piece = creationPiece(ROI, couleur);
        Echiquier[3][7]->piece->x = 3;
        Echiquier[3][7]->piece->y = 7;
        Joueurs[couleur] = Echiquier[3][7]->piece;

        // Reine
        Echiquier[4][7]->piece = creationPiece(REINE, couleur);
        Echiquier[4][7]->piece->x = 4;
        Echiquier[4][7]->piece->y = 7;

        // Circulaire Roi - Reine
        Joueurs[couleur]->pieceSuivante = Echiquier[4][7]->piece;
        Joueurs[couleur]->piecePrecedente = Joueurs[couleur]->pieceSuivante;
        Joueurs[couleur]->piecePrecedente->piecePrecedente = Joueurs[couleur];
        Joueurs[couleur]->piecePrecedente->pieceSuivante = Joueurs[couleur];

        // Insertions Fous
        Echiquier[2][7]->piece = creationPiece(FOU, couleur);
        Echiquier[2][7]->piece->x = 2;
        Echiquier[2][7]->piece->y = 7;
        insertionPieceJoueur(Echiquier[2][7]->piece, Joueurs);
        Echiquier[5][7]->piece = creationPiece(FOU, couleur);
        Echiquier[5][7]->piece->x = 5;
        Echiquier[5][7]->piece->y = 7;
        // Insertions Cavaliers
        insertionPieceJoueur(Echiquier[5][7]->piece, Joueurs);
        Echiquier[1][7]->piece = creationPiece(CAVALIER, couleur);
        Echiquier[1][7]->piece->x = 1;
        Echiquier[1][7]->piece->y = 7;
        insertionPieceJoueur(Echiquier[1][7]->piece, Joueurs);
        Echiquier[6][7]->piece = creationPiece(CAVALIER, couleur);
        Echiquier[6][7]->piece->x = 6;
        Echiquier[6][7]->piece->y = 7;
        // Insertions Tours
        insertionPieceJoueur(Echiquier[6][7]->piece, Joueurs);
        Echiquier[0][7]->piece = creationPiece(TOUR, couleur);
        Echiquier[0][7]->piece->x = 0;
        Echiquier[0][7]->piece->y = 7;
        insertionPieceJoueur(Echiquier[0][7]->piece, Joueurs);
        Echiquier[7][7]->piece = creationPiece(TOUR, couleur);
        Echiquier[7][7]->piece->x = 7;
        Echiquier[7][7]->piece->y = 7;
        insertionPieceJoueur(Echiquier[7][7]->piece, Joueurs);
        // Insertions Pions
        for (int x = 0; x < 8; x++) {
            Echiquier[x][6]->piece = creationPiece(PION, couleur);
            Echiquier[x][6]->piece->x = x;
            Echiquier[x][6]->piece->y = 6;
            insertionPieceJoueur(Echiquier[x][6]->piece, Joueurs); 
        }
    }
}
