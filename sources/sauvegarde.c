#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS

#include ".././headers/sauvegarde.h"

int sauvegarderEchiquier(Case* Echiquier[8][8], Piece* joueurBlanc[16], Piece* joueurNoir[16], int couleurJoueurCourant, char* dossierDeSauvegarde) {
    char repertoireCourant[1024];
    if (getcwd(repertoireCourant, sizeof(repertoireCourant)) == NULL) {
        perror("Impossible de récupérer le répertoire courant");
        return EXIT_FAILURE;
    }

    printf("%s", repertoireCourant);

    // Création du répertoire de sauvegarde si il n'existe pas déjà
    mkdir(dossierDeSauvegarde, 0777);
    
    // Entrer dans le répertoire de sauvegarde
    if (chdir(dossierDeSauvegarde) != 0) {
        perror("Impossible de changer de répertoire");
        return EXIT_FAILURE;
    }

    // Enregistrement de l'état de l'échiquier
    FILE* fichierEchiquier = fopen("echiquier", "w");
    fprintf(fichierEchiquier, "%d\n", couleurJoueurCourant); // La première ligne stocke le joueur courant
    for (int y = 0; y < 8; y++) { // Les lignes suivantes stockent l'état des cases de l'échiquier
        for (int x = 0; x < 8; x++) {
            Case* caseCourante = Echiquier[y][x];
            if (caseCourante->piece != NULL) {
                Piece* piece = caseCourante->piece;
                fprintf(fichierEchiquier, "%d %d %d %d\n", 
                        piece->role, 
                        piece->couleur, 
                        piece->x, 
                        piece->y);
            } else {
                fprintf(fichierEchiquier, "-1 -1 -1 -1\n"); // Case vide
            }
        }
    }

    fclose(fichierEchiquier);

    // Enregistrement des pièces du joueur blanc
    FILE* fichierJoueurBlanc = fopen("joueurBlanc", "w");
    for (int i = 0; i < 16; i++) { // Stockage des états des pièces du joueur blanc
        Piece* piece = joueurBlanc[i];
        if (piece) {
            fprintf(fichierJoueurBlanc, "%d %d %d %d\n", piece->role, piece->couleur, piece->x, piece->y);
        }
    }
    fclose(fichierJoueurBlanc);

    // Enregistrement des pièces du joueur noir
    FILE* fichierJoueurNoir = fopen("joueurNoir", "w");
    for (int i = 0; i < 16; i++) { // Stockage des états des pièces du joueur noir
        Piece* piece = joueurNoir[i];
        if (piece) {
            fprintf(fichierJoueurNoir, "%d %d %d %d\n", piece->role, piece->couleur, piece->x, piece->y);
        }
    }
    fclose(fichierJoueurNoir);

    printf("Enregistrements dans le répertoire \"%s\" réalisé avec succès\n", dossierDeSauvegarde);

    // Retourne au répertoire d'origine
    if (chdir(repertoireCourant) != 0) {
        perror("Impossible de retourner au répertoire d'origine");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int chargerEchiquier(Case* Echiquier[8][8], Piece* joueurBlanc[16], Piece* joueurNoir[16],  int couleurJoueurCourant, char* dossierDeSauvegarde) {
    char repertoireCourant[1024];
    if (getcwd(repertoireCourant, sizeof(repertoireCourant)) == NULL) {
        perror("Impossible de récupérer le répertoire courant");
        return EXIT_FAILURE;
    }

    // Entrer dans le répertoire de sauvegarde
    if (chdir(dossierDeSauvegarde) != 0) {
        perror("Impossible de changer de répertoire");
        return EXIT_FAILURE;
    }

    FILE* fichierEchiquier = fopen("echiquier", "r");
    if (!fichierEchiquier) {
        perror("Impossible d'ouvrir le fichier de sauvegarde");
        if (chdir(repertoireCourant) != 0) {
            perror("Impossible de retourner au répertoire d'origine");
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }

    // Charger l'état de l'échiquier
    fscanf(fichierEchiquier, "%d\n", &couleurJoueurCourant);
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            int role, couleur, posX, posY;
            fscanf(fichierEchiquier, "%d %d %d %d\n", &role, &couleur, &posX, &posY);

            Case* currentCase = creationCase(x, y);
            if (role != -1) {
                Piece* piece = creationPiece((Role)role, couleur);
                piece->x = posX;
                piece->y = posY;
                currentCase->piece = piece;
            }
            Echiquier[y][x] = currentCase;
        }
    }

    fclose(fichierEchiquier);

    // Charger les pièces du joueur blanc
    FILE* fichierJoueurBlanc = fopen("joueurBlanc", "r");
    if (!fichierJoueurBlanc) {
        perror("Impossible d'ouvrir le fichier de sauvegarde");
        if (chdir(repertoireCourant) != 0) {
            perror("Impossible de retourner au répertoire d'origine");
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }
    for (int i = 0; i < 16; i++) {
        int role, couleur, x, y;
        fscanf(fichierJoueurBlanc, "%d %d %d %d\n", &role, &couleur, &x, &y);
        joueurBlanc[i] = creationPiece((Role)role, couleur);
        joueurBlanc[i]->x = x;
        joueurBlanc[i]->y = y;
    }
    fclose(fichierJoueurBlanc);
    // Charger les pièces du joueur noir
    FILE* fichierJoueurNoir = fopen("joueurNoir", "r");
    if (!fichierJoueurNoir) {
        perror("Impossible d'ouvrir le fichier de sauvegarde");
        if (chdir(repertoireCourant) != 0) {
            perror("Impossible de retourner au répertoire d'origine");
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }
    for (int i = 0; i < 16; i++) {
        int role, couleur, x, y;
        fscanf(fichierJoueurNoir, "%d %d %d %d\n", &role, &couleur, &x, &y);
        joueurNoir[i] = creationPiece((Role)role, couleur);
        joueurNoir[i]->x = x;
        joueurNoir[i]->y = y;
    }
    fclose(fichierJoueurNoir);

    printf("Chargement depuis le réprtoire \"%s\" réalisé avec succès\n", dossierDeSauvegarde);

    // Retourne au répertoire d'origine
    if (chdir(repertoireCourant) != 0) {
        perror("Impossible de retourner au répertoire d'origine");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
