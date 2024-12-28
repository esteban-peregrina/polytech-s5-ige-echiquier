#include <stdio.h> // printf()
#include <stdlib.h> // malloc(), free(), exit()

#include ".././include/affichage.h" // afficheEchiquier()
#include ".././include/lecture.h" // set_terminal_raw_mode(), reset_terminal_mode()
#include ".././include/sauvegarde.h" // save_echiquier(), load_echiquier()
#include ".././include/joueur.h" // initialiseJoueur(), videJoueur()

#include ".././include/echiquier.h"

Case* creationCase(int x, int y) {
    /*
    Renvoie l'adresse d'une case vide dont la couleur est specifiée.
    */

    Case* caseCree = NULL;
    caseCree = malloc(sizeof(Case));
    if (caseCree == NULL) { exit(EXIT_FAILURE); }
    
    caseCree->estAtteignableParPiece = false;
    caseCree->estAtteignableParJoueur[NOIR] = 0;
    caseCree->estAtteignableParJoueur[BLANC] = 0;
    caseCree->estSelectionnee = false;

    caseCree->x = x;
    caseCree->y = y;

    return caseCree;
}

void initialiseEchiquier(Case* Echiquier[8][8]) {
    /*
    Rempli l'echiquier de cases vides aux couleurs alternées.
    */

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Echiquier[i][j] = creationCase(i, j);
        }
    }
}

void videEchiquier(Case* Echiquier[8][8]) {
    /*
    Libère la mémoire allouée pour les cases.
    */

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            free(Echiquier[i][j]);
            Echiquier[i][j] = NULL; // Libère la mémoire allouée pour les cases
        }
    }
}

void partieEchec(Case* Echiquier[8][8], Piece *Blancs[16], Piece *Noirs[16], int couleurJoueurCourant) {
    /*
    Démarre une partie d'échec depuis l'état de l'echiquier et du point de vu du joueur courant.
    */
    Piece **joueurCourant, **joueurAdverse;

    if (couleurJoueurCourant == BLANC) {
        joueurCourant = Blancs;
        joueurAdverse = Noirs;
    } else {
        joueurCourant = Noirs;
        joueurAdverse = Blancs;
    }

    bool echecEtMat = false;

    while (!echecEtMat) {
        actualiseCasesAtteignablesParJoueur(Echiquier, joueurCourant); // On actualise chaque tableau des cases atteignables par les pièces du joueur
        actualiseExposeRoi(Echiquier, joueurCourant, joueurAdverse); // On retire les cases exposant le roi allié à l'échec

        bool aJoue = false;
        Menu menu = PIECES; // On commence le tour en sélectionnant une pièce

        // On sélectionne la première pièce non bloquée et non capturée
        int indicePieceCourante = 0;
        while ( (indicePieceCourante < 16) && ((joueurCourant[indicePieceCourante]->estBloquee) || (joueurCourant[indicePieceCourante]->estCapturee)) ) { indicePieceCourante++; }
        if (indicePieceCourante >= 16) { break; } // Echec et mat (aucune pièce disponible)
        Piece* pieceCourante = joueurCourant[indicePieceCourante];
        pieceCourante->estSelectionnee = true;
        
        // On actualise l'échiquier pour afficher les cases atteignables par la pièce courante
        Piece* piecePrecedente = NULL;
        actualiseCasesAtteignablesParPiece(pieceCourante, piecePrecedente);

        // On déclare ici pour le compilateur
        int indiceCaseCourante = 0;
        Case* caseCourante = NULL;

        while (!aJoue) {
            afficheEchiquier(Echiquier);
            printf("Sélectionnez une pièce à jouer à l'aide des touches directionnelles (appuyez sur 'q' pour quitter)\n");

            // Sauvegarder les paramètres originaux du terminal
            struct termios orig_termios;
            tcgetattr(STDIN_FILENO, &orig_termios);

            // Configurer le terminal en mode "raw"
            set_terminal_raw_mode();

            char actionJoueur;
            if (read(STDIN_FILENO, &actionJoueur, 1) != 1) { exit(EXIT_FAILURE); } // Écrit l'entrée utilisateur lue dans &actionJoueur et vérifie que cela à fonctionné

            if (actionJoueur == 'q') {
                pieceCourante->estSelectionnee = false;
                if (caseCourante != NULL) { caseCourante->estSelectionnee = false; }
                
                reset_terminal_mode(&orig_termios);
                printf("\n");

                char reponse[4]; // 3 caractères et le caractère de fin de chaine
                printf("Voulez-vous sauvegarder la partie ? (o/n) : ");
                scanf("%3s", reponse);
                
                bool reponseValide = false;
                while (!reponseValide) {
                    if (reponse[0] == 'o') {
                        printf("Enregistrement de la partie...\n");
                        // Initialisations par chargement
                        if (sauvegarderEchiquier(Blancs, Noirs, couleurJoueurCourant, "sauvegardes") == EXIT_FAILURE) {
                            printf("Sauvegarde impossible.\n");
                            reponse[0] = 'n'; // On commence une nouvelle partie
                        } else { reponseValide = true; }
                    } else if (reponse[0] == 'n') {
                        printf("Sortie de la partie\n");
                        reponseValide = true;
                    } else {
                        printf("Réponse incorrecte. Veuillez répondre par 'o' ou 'n' : ");
                        scanf("%3s", reponse);
                    }
                }

                videEchiquier(Echiquier); // Libère les cases
                videJoueur(Blancs); // Libère les pièces blanches
                videJoueur(Noirs);  // Libère les pièces noires  
                exit(EXIT_SUCCESS);
            }

            if (menu == PIECES) {
                if (actionJoueur == '\033') {
                    char seq[2];
                    if (read(STDIN_FILENO, &seq[0], 1) != 1 || read(STDIN_FILENO, &seq[1], 1) != 1) { exit(EXIT_FAILURE); }

                    if (seq[0] == '[') {
                        switch (seq[1]) {
                            case 'B': // Flèche bas
                                // On sélectionne la première case atteignable (il y en a forcément une comme la pièce n'est pas bloquée)
                                indiceCaseCourante = 0;
                                caseCourante = pieceCourante->casesAtteignables[indiceCaseCourante];
                                caseCourante->estSelectionnee = true;
                                pieceCourante->estSelectionnee = true;
                                menu = COUPS;
                                break;
                            case 'C': // Flèche droite
                                do {
                                    indicePieceCourante = (indicePieceCourante == 15) ? 0 : indicePieceCourante + 1;
                                } while (joueurCourant[indicePieceCourante]->estBloquee || joueurCourant[indicePieceCourante]->estCapturee);

                                pieceCourante->estSelectionnee = false;
                                piecePrecedente = pieceCourante;
                                pieceCourante = joueurCourant[indicePieceCourante]; 
                                pieceCourante->estSelectionnee = true;
                                actualiseCasesAtteignablesParPiece(pieceCourante, piecePrecedente);
                                break;
                            case 'D': // Flèche gauche
                                do {
                                    indicePieceCourante = (indicePieceCourante == 0) ? 15 : indicePieceCourante - 1;
                                } while (joueurCourant[indicePieceCourante]->estBloquee || joueurCourant[indicePieceCourante]->estCapturee);
                                
                                pieceCourante->estSelectionnee = false;
                                piecePrecedente = pieceCourante;
                                pieceCourante = joueurCourant[indicePieceCourante]; 
                                pieceCourante->estSelectionnee = true;
                                actualiseCasesAtteignablesParPiece(pieceCourante, piecePrecedente);
                                break;
                            default:
                                printf("Caractère incorrect\n");
                                break;
                        }
                    } else {
                        printf("Autre caractère spécial\n");
                    }
                }
            } else { //menu == COUPS
                if (actionJoueur == '\033') {
                    char seq[2];
                    if (read(STDIN_FILENO, &seq[0], 1) != 1 || read(STDIN_FILENO, &seq[1], 1) != 1) { exit(EXIT_FAILURE); }

                    if (seq[0] == '[') {
                        switch (seq[1]) {
                            case 'A': // Flèche haut
                                caseCourante->estSelectionnee = false;
                                pieceCourante->estSelectionnee = true;
                                menu = PIECES;
                                break;
                            case 'C': // Flèche droite
                                indiceCaseCourante = (indiceCaseCourante == pieceCourante->longueurCasesAtteignables - 1) ? 0 : indiceCaseCourante + 1;
                                caseCourante->estSelectionnee = false;
                                caseCourante = pieceCourante->casesAtteignables[indiceCaseCourante];
                                caseCourante->estSelectionnee = true;
                                break;
                            case 'D': // Flèche gauche
                                indiceCaseCourante = (indiceCaseCourante == 0) ? pieceCourante->longueurCasesAtteignables - 1 : indiceCaseCourante - 1;
                                caseCourante->estSelectionnee = false;
                                caseCourante = pieceCourante->casesAtteignables[indiceCaseCourante];
                                caseCourante->estSelectionnee = true;
                                break;
                            default:
                                printf("Caractère incorrect\n");
                                break;
                        }
                    } else {
                        printf("Autre caractère spécial\n");
                    }
                } else if (actionJoueur == '\n') { // Entrée - Le coup est validé
                    int xPrecedent = pieceCourante->x;
                    int yPrecedent = pieceCourante->y;
                    
                    int xCible = caseCourante->x;
                    int yCible = caseCourante->y;
                    Piece* contenuCaseCible = caseCourante->piece; // On garde en mémoire la pièce présente sur la case cible

                    // Gestion du coup
                    Echiquier[xPrecedent][yPrecedent]->piece = NULL; // On enlève la pièce courante de sa positon précédente
                    if (contenuCaseCible != NULL) { contenuCaseCible->estCapturee = true; } // On capture la pièce si elle existe (on a seulement accès à des cases adverses)
                    caseCourante->piece = pieceCourante; // Écrase l'adrese de la pièce capturée avec celle de la pièce alliée
                    pieceCourante->x = xCible; // On met à jours les coordonnées de la pièce déplacée
                    pieceCourante->y = yCible;

                    caseCourante->estSelectionnee = false;
                    pieceCourante->estSelectionnee = false;
                    menu = PIECES;
                    aJoue = true;
                    
                    couleurJoueurCourant = (couleurJoueurCourant == BLANC) ? NOIR : BLANC;

                    // On commute les joueurs
                    joueurCourant = (joueurCourant == Blancs) ? Noirs : Blancs;
                    joueurAdverse = (joueurAdverse == Noirs) ? Blancs : Noirs;
                       
                    // On vide les cases atteignables par la pièce courante pour ne plus les afficher
                    actualiseCasesAtteignablesParPiece(NULL, pieceCourante); // On ne connait pas encore la piece courante de l'adversaire
                }
            }
            // Rétablir les paramètres originaux
            reset_terminal_mode(&orig_termios);
        }
    }
    (joueurCourant == Blancs) ? printf("Victoire des Noirs (Rouges) !\n") : printf("Victoire des Blancs (Bleues) !\n");
    videEchiquier(Echiquier); // Libère les cases
    videJoueur(Blancs); // Libère les pièces blanches
    videJoueur(Noirs);  // Libère les pièces noires  
}

void jeuEchec() {
    /*
    Démarre le jeu d'Echec.
    */
    
    // TODO - Proposer de jouer contre un joueur ou contre une IA

    Case* Echiquier[8][8]; // Déclaration de l'echiquier
    initialiseEchiquier(Echiquier);

    Piece *Blancs[16], *Noirs[16];  // Déclaration des joueurs
    int couleurJoueurCourant = -1;

    char reponse[4]; // 3 caractères et le caractère de fin de chaine
    printf("Voulez-vous charger une sauvegarde ? (o/n) : ");
    scanf("%3s", reponse);

    bool reponseValide = false;
    while (!reponseValide) {
        if (reponse[0] == 'o') {
            printf("Chargement de la sauvegarde...\n");
            // Initialisations par chargement
            if (chargerEchiquier(Echiquier, Blancs, Noirs, &couleurJoueurCourant, "sauvegardes") == EXIT_FAILURE) {
                printf("Fichiers de sauvegarde corrompus. Création d'une nouvelle partie.\n");
                reponse[0] = 'n'; // On commence une nouvelle partie
            } else { reponseValide = true; }
        } else if (reponse[0] == 'n') {
            printf("Nouvelle partie...\n");
            // Initialisations par défaut
            initialiseJoueur(Echiquier, Blancs, BLANC);
            initialiseJoueur(Echiquier, Noirs, NOIR);
            couleurJoueurCourant = BLANC; // Les blancs commencent
            reponseValide = true;
        } else {
            printf("Réponse incorrecte. Veuillez répondre par 'o' ou 'n' : ");
            scanf("%3s", reponse);
        }
    }

    actualiseCasesAtteignablesParJoueur(Echiquier, Blancs);
    actualiseCasesAtteignablesParJoueur(Echiquier, Noirs);
    partieEchec(Echiquier, Blancs, Noirs, couleurJoueurCourant);
}