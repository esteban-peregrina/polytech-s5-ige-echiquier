#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS

#include ".././include/sauvegarde.h"

int sauvegarderEchiquier(Piece* joueurBlanc[16], Piece* joueurNoir[16], int couleurJoueurCourant, char* dossierDeSauvegarde) {
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
    FILE* fichierEchiquier = fopen("echiquier", "w");
    fprintf(fichierEchiquier, "%d\n", couleurJoueurCourant); // La première ligne stocke le joueur courant
    fclose(fichierEchiquier);

    // Enregistrement des pièces du joueur blanc
    FILE* fichierJoueurBlanc = fopen("joueurBlanc", "w");
    for (int i = 0; i < 16; i++) { // Stockage des états des pièces du joueur blanc
        Piece* piece = joueurBlanc[i];
        if (piece) {
            fprintf(fichierJoueurBlanc, "%d %d %d %d %d\n", piece->role, piece->couleur, piece->estCapturee, piece->x, piece->y);
        }
    }
    fclose(fichierJoueurBlanc);

    // Enregistrement des pièces du joueur noir
    FILE* fichierJoueurNoir = fopen("joueurNoir", "w");
    for (int i = 0; i < 16; i++) { // Stockage des états des pièces du joueur noir
        Piece* piece = joueurNoir[i];
        if (piece) {
            fprintf(fichierJoueurNoir, "%d %d %d %d %d\n", piece->role, piece->couleur, piece->estCapturee, piece->x, piece->y);
        }
    }
    fclose(fichierJoueurNoir);

    printf("Enregistrements dans le répertoire \"%s\" réalisé avec succès\n", dossierDeSauvegarde);

    // Retour au répertoire d'origine
    if (chdir(repertoireCourant) != 0) {
        perror("Impossible de retourner au répertoire d'origine");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int chargerEchiquier(Case* Echiquier[8][8], Piece* joueurBlanc[16], Piece* joueurNoir[16], int* couleurJoueurCourant, char* dossierDeSauvegarde) {
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

    FILE* fichierEchiquier = fopen("echiquier", "r");
    if (!fichierEchiquier) {
        perror("Impossible d'ouvrir le fichier de sauvegarde");
        if (chdir(repertoireCourant) != 0) {
            perror("Impossible de retourner au répertoire d'origine");
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }

    // Chargement de l'état de l'échiquier
    fscanf(fichierEchiquier, "%d\n", couleurJoueurCourant);
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
        int role, couleur, estCapturee, x, y;
        fscanf(fichierJoueurBlanc, "%d %d %d %d %d\n", &role, &couleur, &estCapturee, &x, &y);
        joueurBlanc[i] = creationPiece((Role)role, couleur);
        joueurBlanc[i]->x = x;
        joueurBlanc[i]->y = y;
        joueurBlanc[i]->estCapturee = (bool)estCapturee;
        Echiquier[x][y]->piece = joueurBlanc[i];
    }
    fclose(fichierJoueurBlanc);

    // Chargement des pièces du joueur noir
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
        int role, couleur, estCapturee, x, y;
        fscanf(fichierJoueurNoir, "%d %d %d %d %d\n", &role, &couleur, &estCapturee, &x, &y);
        joueurNoir[i] = creationPiece((Role)role, couleur);
        joueurNoir[i]->x = x;
        joueurNoir[i]->y = y;
        joueurNoir[i]->estCapturee = (bool)estCapturee;
        Echiquier[x][y]->piece = joueurNoir[i];
    }
    fclose(fichierJoueurNoir);

    printf("Chargement depuis le réprtoire \"%s\" réalisé avec succès\n", dossierDeSauvegarde);

    // Retour au répertoire d'origine
    if (chdir(repertoireCourant) != 0) {
        perror("Impossible de retourner au répertoire d'origine");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
