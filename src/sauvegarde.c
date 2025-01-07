#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS

#include ".././include/timer.h" // couleurJoueurCourant

#include ".././include/sauvegarde.h"

int sauvegarderEchiquier(Piece* joueurBlanc[16], int scoreBlancs, Piece* joueurNoir[16], int scoreNoirs, int couleurJoueurCourant, char* dossierDeSauvegarde) {
    char repertoireCourant[1024];
    if (getcwd(repertoireCourant, sizeof(repertoireCourant)) == NULL) {
        perror("Impossible de récupérer le répertoire courant");
        return EXIT_FAILURE;
    }

    // Création du répertoire de sauvegarde si il n'existe pas déjà
    mkdir(dossierDeSauvegarde, 0777);
    
    // Déplacement au répertoire de sauvegarde
    if (chdir(dossierDeSauvegarde) != 0) {
        perror("Impossible de changer de répertoire");
        return EXIT_FAILURE;
    }

    // Enregistrement de l'état de l'échiquier
    FILE* fichierSauvegarde = fopen("sauvegarde", "w");
    fprintf(fichierSauvegarde, "%d %d %d\n", couleurJoueurCourant, scoreBlancs, scoreNoirs);
   
    for (int joueur = 0; joueur < 2; joueur++) {
        Piece** joueurCourant = NULL;
        switch (joueur)
        {
        case 0:
            joueurCourant = joueurNoir;
            break;

        case 1:
            joueurCourant = joueurBlanc;
            break;
        }

        for (int p = 0; p < 16; p++) { // Stockage des états des pièces du joueur courant
            Piece* piece = joueurCourant[p];
            if (piece) {
                fprintf(fichierSauvegarde, "%d %d %d %d %d %d %d\n", 
                piece->role, 
                piece->couleur, 
                piece->estCapturee, 
                piece->vientDeFaireDoublePas, 
                piece->aPrecedemmentBouge, 
                piece->x, 
                piece->y);
            }
        }
    }

    fclose(fichierSauvegarde);

    printf("Enregistrement dans le répertoire \"%s\" réalisé avec succès\n", dossierDeSauvegarde);

    // Retour au répertoire d'origine
    if (chdir(repertoireCourant) != 0) {
        perror("Impossible de retourner au répertoire d'origine");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int chargerEchiquier(Case* Echiquier[8][8], Piece* joueurBlanc[16], int* scoreBlancs, Piece* joueurNoir[16], int* scoreNoirs, char* dossierDeSauvegarde) {
    char repertoireCourant[1024];
    if (getcwd(repertoireCourant, sizeof(repertoireCourant)) == NULL) {
        perror("Impossible de récupérer le répertoire courant");
        return EXIT_FAILURE;
    }

    // Déplacement au répertoire de sauvegarde
    if (chdir(dossierDeSauvegarde) != 0) {
        perror("Impossible de changer de répertoire");
        return EXIT_FAILURE;
    }

    FILE* fichierSauvegarde = fopen("sauvegarde", "r");
    if (!fichierSauvegarde) {
        perror("Impossible d'ouvrir le fichier de sauvegarde");
        if (chdir(repertoireCourant) != 0) {
            perror("Impossible de retourner au répertoire d'origine");
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }

    // Chargement de l'état de l'échiquier
    fscanf(fichierSauvegarde, "%d %d %d\n", &couleurJoueurCourant, scoreBlancs, scoreNoirs);

    for (int joueur = 0; joueur < 2; joueur++) {
        Piece** joueurCourant = NULL;
        switch (joueur)
        {
        case 0:
            joueurCourant = joueurNoir;
            break;

        case 1:
            joueurCourant = joueurBlanc;
            break;
        }

        for (int p = 0; p < 16; p++) { // Stockage des états des pièces du joueur courant
            int role, couleur, estCapturee, vientDeFaireDoublePas, aPrecedemmentBouge, x, y;
            fscanf(fichierSauvegarde, "%d %d %d %d %d %d %d\n", 
            &role, 
            &couleur, 
            &estCapturee, 
            &vientDeFaireDoublePas, 
            &aPrecedemmentBouge, 
            &x, 
            &y);

            Piece* piece = creationPiece((Role)role, couleur);
            piece->estCapturee = (bool)estCapturee;
            piece->vientDeFaireDoublePas = (bool)vientDeFaireDoublePas;
            piece->aPrecedemmentBouge = (bool)aPrecedemmentBouge;
            piece->x = x;
            piece->y = y;
            joueurCourant[p] = piece;
            if (!piece->estCapturee) { Echiquier[x][y]->piece = piece; }
        }
    }

    fclose(fichierSauvegarde);

    printf("Chargement depuis le répertoire \"%s\" réalisé avec succès\n", dossierDeSauvegarde);

    // Retour au répertoire d'origine
    if (chdir(repertoireCourant) != 0) {
        perror("Impossible de retourner au répertoire d'origine");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
