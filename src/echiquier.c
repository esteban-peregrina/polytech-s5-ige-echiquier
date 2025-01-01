#include <stdio.h> // printf()
#include <stdlib.h> // malloc(), free(), exit()

#include ".././include/affichage.h" // afficheEchiquier()
#include ".././include/lecture.h" // set_terminal_raw_mode(), reset_terminal_mode()
#include ".././include/sauvegarde.h" // save_echiquier(), load_echiquier()
#include ".././include/joueur.h" // initialiseJoueur(), videJoueur()
#include ".././include/mouvement.h" // initialiseJoueur(), videJoueur()
#include ".././include/calcul_atteignable.h" // calculAtteignableCavalier(), calculAtteignableTour(), calculAtteignableFou(), calculAtteignableReine()

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
    Rempli l'echiquier de cases vides.
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

    // --- Initialisation --- //
    Piece **joueurCourant, **joueurAdverse;

    if (couleurJoueurCourant == BLANC) {
        joueurCourant = Blancs;
        joueurAdverse = Noirs;
    } else {
        joueurCourant = Noirs;
        joueurAdverse = Blancs;
    }

    // --- Partie --- //
    while (true) {
        actualiseCasesAtteignablesParJoueur(Echiquier, joueurCourant); // On actualise chaque tableau des cases absolument atteignables par les pièces du joueur
        actualiseExposeRoi(Echiquier, joueurCourant, joueurAdverse); // On retire les cases exposant le roi allié à l'échec

        bool aJoue = false;
        Menu menu = PIECES; // On commence le tour en sélectionnant une pièce

        // On fait sélectionner la première pièce non bloquée et non capturée
        int indicePieceCourante = 0;
        while ( (indicePieceCourante < 16) && ((joueurCourant[indicePieceCourante]->estBloquee) || (joueurCourant[indicePieceCourante]->estCapturee)) ) { indicePieceCourante++; }
        if (indicePieceCourante >= 16) { break; } // Fin de la partie
        Piece* pieceCourante = joueurCourant[indicePieceCourante];
        pieceCourante->estSelectionnee = true;
        
        // On actualise l'échiquier pour afficher les cases atteignables par la pièce courante
        Piece* piecePrecedente = NULL;
        actualiseCasesAtteignablesParPiece(pieceCourante, piecePrecedente);

        // (Déclarés ici pour le compilateur)
        int indiceCaseCourante = 0;
        Case* caseCourante = NULL;

        // --- Actions du joueur --- //
        while (!aJoue) {
            afficheEchiquier(Echiquier);
            printf("Sélectionnez une pièce à jouer à l'aide des touches directionnelles (appuyez sur 'q' pour quitter)\n");

            // Sauvegarder les paramètres originaux du terminal
            struct termios orig_termios;
            tcgetattr(STDIN_FILENO, &orig_termios);

            // Configurer le terminal en mode "raw"
            set_terminal_raw_mode();

            char actionJoueur;
            if (read(STDIN_FILENO, &actionJoueur, 1) != 1) { exit(EXIT_FAILURE); } // Écrit l'entrée utilisateur lue dans &actionJoueur et vérifie que cela ai fonctionné

            // Le joueur décide de quitter la partie
            if (actionJoueur == 'q') {
                pieceCourante->estSelectionnee = false;
                if (caseCourante != NULL) { caseCourante->estSelectionnee = false; }
                
                reset_terminal_mode(&orig_termios);

                char reponse; 
                printf("Voulez-vous sauvegarder la partie ? (o/n) : ");
                scanf("%c", &reponse);
                
                bool reponseValide = false;
                while (!reponseValide) {
                    if (reponse == 'o') {
                        printf("Enregistrement de la partie...\n");
                        // Initialisations par chargement
                        if (sauvegarderEchiquier(Blancs, Noirs, couleurJoueurCourant, "sauvegardes") == EXIT_FAILURE) {
                            printf("Sauvegarde impossible.\n");
                            reponse = 'n'; // On commence une nouvelle partie
                        } else { reponseValide = true; }
                    } else if (reponse == 'n') {
                        printf("Sortie de la partie\n");
                        reponseValide = true;
                    } else {
                        printf("Réponse incorrecte. Veuillez répondre par 'o' ou 'n' : ");
                        scanf("%c", &reponse);
                    }
                }

                videEchiquier(Echiquier); // Libère les cases
                videJoueur(Blancs); // Libère les pièces blanches
                videJoueur(Noirs);  // Libère les pièces noires  
                exit(EXIT_SUCCESS);
            }

            // Lorsque le joueur sélectionne une pièce
            if (menu == PIECES) {
                if (actionJoueur == '\033') { // Caractère spécial
                    char seq[2]; // Séquence de caractères
                    if (read(STDIN_FILENO, &seq[0], 1) != 1 || read(STDIN_FILENO, &seq[1], 1) != 1) { exit(EXIT_FAILURE); }

                    if (seq[0] == '[') { // Début d'une séquence de caractères pour les flèches
                        switch (seq[1]) {

                            // Lorsque le joueur valide une pièce
                            case 'B': // Flèche bas
                                // On sélectionne la première case atteignable (il y en a forcément une comme la pièce n'est pas bloquée)
                                indiceCaseCourante = 0;
                                caseCourante = pieceCourante->casesAtteignables[indiceCaseCourante];
                                caseCourante->estSelectionnee = true;
                                pieceCourante->estSelectionnee = true;
                                menu = COUPS;
                                break;

                            // Lorque le joueur navigue parmi ses pièces disponibles
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

                            // Lorsque le chat du joueur marche sur son clavier
                            default:
                                printf("Caractère incorrect\n");
                                break;
                        }
                    } else {
                        // Lorsque le chat du joueur marche sur son clavier
                        printf("Caractère incorrect\n");
                    }
                }
            // Lorque le joueur sélectionne une case atteignable par la pièce validée
            } else { //menu == COUPS
                if (actionJoueur == '\033') {
                    char seq[2];
                    if (read(STDIN_FILENO, &seq[0], 1) != 1 || read(STDIN_FILENO, &seq[1], 1) != 1) { exit(EXIT_FAILURE); }

                    if (seq[0] == '[') {
                        switch (seq[1]) {
                            // Lorsque le retourne à la sélection des pièces
                            case 'A': // Flèche haut
                                caseCourante->estSelectionnee = false;
                                pieceCourante->estSelectionnee = true;
                                menu = PIECES;
                                break;
                        
                            // Lorque le joueur navigue parmi les cases atteignables
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

                            // Lorsque le chat du joueur marche sur son clavier
                            default:
                                printf("Caractère incorrect\n");
                                break;
                        }
                    } else {
                        // Lorsque le chat du joueur marche sur son clavier
                        printf("Caractère incorrect\n");
                    }
                
                // Lorque le joueur valide une case atteignable
                } else if (actionJoueur == '\n') { // Entrée - Le coup est validé
                    // Gestion du coup
                    mouvement(Echiquier, pieceCourante, caseCourante, false); // On effectue le mouvement

                    // Gestions des promotions
                    int rangeePromotion = (couleurJoueurCourant == BLANC) ? 7 : 0;
                    if ((pieceCourante->role == PION) && (caseCourante->x == rangeePromotion)) {
                         char reponse;
                        printf("Promotion du pion ! Choisissez une pièce (R,F,T,C) : ");
                        scanf("%c", &reponse);
                        
                        bool reponseValide = false;
                        while (!reponseValide) {
                            if (reponse == 'R') {
                                printf("Promotion en Reine...\n");
                                pieceCourante->role = REINE;
                                pieceCourante->forme = "♛";
                                pieceCourante->calculAtteignable = calculAtteignableReine;
                                
                                reponseValide = true;
                            } else if (reponse == 'F') {
                                printf("Promotion en Fou...\n");
                                pieceCourante->role = FOU;
                                pieceCourante->forme = "♝";
                                pieceCourante->calculAtteignable = calculAtteignableFou;
                                
                                reponseValide = true;
                            } else if (reponse == 'T') {
                                printf("Promotion en Tour...\n");
                                pieceCourante->role = TOUR;
                                pieceCourante->forme = "♜";
                                pieceCourante->calculAtteignable = calculAtteignableTour;
                                
                                reponseValide = true;
                            
                            } else if (reponse == 'C') {
                                printf("Promotion en Cavalier...\n");
                                pieceCourante->role = CAVALIER;
                                pieceCourante->forme = "♞";
                                pieceCourante->calculAtteignable = calculAtteignableCavalier;
                                
                                reponseValide = true;
                            } else {
                                printf("Réponse incorrecte. Veuillez répondre par 'R','F','T' ou 'C' : ");
                                scanf("%c", &reponse);
                            }
                        }
                    }

                    // On réinitialise les paramètres de sélection
                    caseCourante->estSelectionnee = false;
                    pieceCourante->estSelectionnee = false;

                    // On donne la main à l'adversaire
                    menu = PIECES;
                    aJoue = true;
                    
                    couleurJoueurCourant = (couleurJoueurCourant == BLANC) ? NOIR : BLANC;

                    // On commute les joueurs
                    joueurCourant = (joueurCourant == Blancs) ? Noirs : Blancs;
                    joueurAdverse = (joueurAdverse == Noirs) ? Blancs : Noirs;
                       
                    // On vide les cases atteignables par la pièce courante pour ne plus les afficher (On ne connait pas encore la piece courante de l'adversaire)
                    actualiseCasesAtteignablesParPiece(NULL, pieceCourante);
                }
            }
            // On rétablit les paramètres originaux
            reset_terminal_mode(&orig_termios);
        }
    }
    // Gestion du pat ou échec et mat
    afficheEchiquier(Echiquier);
    Case* caseRoyale = Echiquier[joueurCourant[4]->x][joueurCourant[4]->y];
    if (couleurJoueurCourant == BLANC) {
        if (caseRoyale->estAtteignableParJoueur[joueurAdverse[4]->couleur] > 0) { printf("Échec et mat : Victoire des Noirs (Rouges) !\n"); }
        else { printf("Pat !\n"); }
    } else {
        if (caseRoyale->estAtteignableParJoueur[joueurAdverse[4]->couleur] > 0) { printf("Échec et mat : Victoire des Blancs (Bleus) !\n"); } 
        else { printf("Pat !\n"); }
    }
        
    videEchiquier(Echiquier); // Libère les cases
    videJoueur(Blancs); // Libère les pièces blanches
    videJoueur(Noirs);  // Libère les pièces noires 
    
    printf("Partie terminée !\n"); 
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

    char reponse; 
    printf("Voulez-vous charger une sauvegarde ? (o/n) : ");
    scanf("%c", &reponse);

    bool reponseValide = false;
    while (!reponseValide) {
        if (reponse == 'o') {
            printf("Chargement de la sauvegarde...\n");
            // Initialisations par chargement
            if (chargerEchiquier(Echiquier, Blancs, Noirs, &couleurJoueurCourant, "sauvegardes") == EXIT_FAILURE) {
                printf("Fichiers de sauvegarde corrompus. Création d'une nouvelle partie.\n");
                reponse = 'n'; // On commence une nouvelle partie
            } else { reponseValide = true; }
        } else if (reponse == 'n') {
            printf("Nouvelle partie...\n");
            // Initialisations par défaut
            initialiseJoueur(Echiquier, Blancs, BLANC);
            initialiseJoueur(Echiquier, Noirs, NOIR);
            couleurJoueurCourant = BLANC; // Les blancs commencent
            reponseValide = true;
        } else {
            printf("Réponse incorrecte. Veuillez répondre par 'o' ou 'n' : ");
            scanf("%c", &reponse);
        }
    }

    actualiseCasesAtteignablesParJoueur(Echiquier, Blancs);
    actualiseCasesAtteignablesParJoueur(Echiquier, Noirs);
    
    partieEchec(Echiquier, Blancs, Noirs, couleurJoueurCourant);
}