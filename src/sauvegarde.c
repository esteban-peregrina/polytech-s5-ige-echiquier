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
            int estCapturee;
            if (piece->estCapturee) {
                estCapturee = 1;
            } else { estCapturee = 0;}
            int vientDeFaireDoublePas;
            if (piece->vientDeFaireDoublePas) {
                vientDeFaireDoublePas = 1;
            } else { vientDeFaireDoublePas = 0;}
            int aPrecedemmentBouge;
            if (piece->aPrecedemmentBouge) {
                aPrecedemmentBouge = 1;
            } else { aPrecedemmentBouge = 0;}
            fprintf(fichierJoueurBlanc, "%d %d %d %d %d %d %d\n", piece->role, piece->couleur, estCapturee, vientDeFaireDoublePas, aPrecedemmentBouge, piece->x, piece->y);
        }
    }
    fclose(fichierJoueurBlanc);

    // Enregistrement des pièces du joueur noir
    FILE* fichierJoueurNoir = fopen("joueurNoir", "w");
    for (int i = 0; i < 16; i++) { // Stockage des états des pièces du joueur noir
        Piece* piece = joueurNoir[i];
        if (piece) {
            int estCapturee;
            if (piece->estCapturee) {
                estCapturee = 1;
            } else { estCapturee = 0;}
            int vientDeFaireDoublePas;
            if (piece->vientDeFaireDoublePas) {
                vientDeFaireDoublePas = 1;
            } else { vientDeFaireDoublePas = 0;}
            int aPrecedemmentBouge;
            if (piece->aPrecedemmentBouge) {
                aPrecedemmentBouge = 1;
            } else { aPrecedemmentBouge = 0;}
            fprintf(fichierJoueurNoir, "%d %d %d %d %d %d %d\n", piece->role, piece->couleur, estCapturee, vientDeFaireDoublePas, aPrecedemmentBouge, piece->x, piece->y);
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
        int role, couleur, estCapturee, vientDeFaireDoublePas, aPrecedemmentBouge, x, y;
        fscanf(fichierJoueurBlanc, "%d %d %d %d %d %d %d\n", &role, &couleur, &estCapturee, &vientDeFaireDoublePas, &aPrecedemmentBouge, &x, &y);
        joueurBlanc[i] = creationPiece((Role)role, couleur);
        joueurBlanc[i]->x = x;
        joueurBlanc[i]->y = y;
        if (estCapturee == 0) {
            joueurBlanc[i]->estCapturee = false;
        } else { joueurBlanc[i]->estCapturee = true;}
        if (vientDeFaireDoublePas == 0) {
            joueurBlanc[i]->vientDeFaireDoublePas = false;
        } else { joueurBlanc[i]->vientDeFaireDoublePas = true;}
        if (aPrecedemmentBouge == 0) {
            joueurBlanc[i]->aPrecedemmentBouge = false;
        } else { joueurBlanc[i]->aPrecedemmentBouge = true;}
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
        int role, couleur, estCapturee, vientDeFaireDoublePas, aPrecedemmentBouge, x, y;
        fscanf(fichierJoueurNoir, "%d %d %d %d %d %d %d\n", &role, &couleur, &estCapturee, &vientDeFaireDoublePas, &aPrecedemmentBouge, &x, &y);
        joueurNoir[i] = creationPiece((Role)role, couleur);
        joueurNoir[i]->x = x;
        joueurNoir[i]->y = y;
        if (estCapturee == 0) {
            joueurNoir[i]->estCapturee = false;
        } else { joueurNoir[i]->estCapturee = true;}
        if (vientDeFaireDoublePas == 0) {
            joueurNoir[i]->vientDeFaireDoublePas = false;
        } else { joueurNoir[i]->vientDeFaireDoublePas = true;}
        if (aPrecedemmentBouge == 0) {
            joueurNoir[i]->aPrecedemmentBouge = false;
        } else { joueurNoir[i]->aPrecedemmentBouge = true;}
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
