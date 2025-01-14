#include <stdio.h> // printf()
#include <stdlib.h> // malloc(), free(), exit()
#include <pthread.h> // pthread_t

#include ".././include/affichage.h" // afficheEchiquier()
#include ".././include/lecture.h" // set_terminal_raw_mode(), reset_terminal_mode()
#include ".././include/sauvegarde.h" // save_echiquier(), load_echiquier()
#include ".././include/joueur.h" // initialiseJoueur(), videJoueur()
#include ".././include/mouvement.h" // initialiseJoueur(), videJoueur()
#include ".././include/calcul_atteignable.h" // calculAtteignableCavalier(), calculAtteignableTour(), calculAtteignableFou(), calculAtteignableReine()
#include ".././include/timer.h" // threadTimer(), doitArreterTimer, couleurJoueurCourant

#include ".././include/echiquier.h"

volatile bool doitArreterTimer = false;
volatile int couleurJoueurCourant = -1;

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

void partieEchec(Case* Echiquier[8][8], Piece *Blancs[16], int* scoreBlancs, Piece *Noirs[16], int* scoreNoirs, int type) {
    /*
    Démarre une partie d'échec depuis l'état de l'echiquier et du point de vu du joueur courant.
    */

    printf("\033[H\033[J"); // Efface le terminal
    pthread_t timerThread;
    // Lance le thread du timer
    if (pthread_create(&timerThread, NULL, threadTimer, NULL) != 0) {
        perror("Echec de la création du thread du timer !\n");
        exit(EXIT_FAILURE);
    }

    // --- Initialisation --- //
    Piece **joueurCourant, **joueurAdverse;
    int* scoreCourant;

    if (couleurJoueurCourant == BLANC) {
        joueurCourant = Blancs;
        joueurAdverse = Noirs;
        scoreCourant = scoreBlancs;
    } else {
        joueurCourant = Noirs;
        joueurAdverse = Blancs;
        scoreCourant = scoreNoirs;
    }
    // --- Partie --- //
    bool finPartie = false;
    while (!finPartie) {
        actualiseCasesAtteignablesParJoueur(Echiquier, joueurAdverse);
        actualiseCasesAtteignablesParJoueur(Echiquier, joueurCourant); // On actualise chaque tableau des cases absolument atteignables par les pièces du joueur
        actualiseExposeRoi(Echiquier, joueurCourant, joueurAdverse); // On retire les cases exposant le roi allié à l'échec

        bool aJoue = false;
        Menu menu = PIECES; // On commence le tour en sélectionnant une pièce

        // On fait sélectionner la première pièce non bloquée et non capturée
        int indicePieceCourante = 0;
        while ( (indicePieceCourante < 16) && ((joueurCourant[indicePieceCourante]->estBloquee) || (joueurCourant[indicePieceCourante]->estCapturee)) ) { indicePieceCourante++; }
        if (indicePieceCourante >= 16) { 
            // Gestion du pat ou échec et mat
            afficheEchiquier(Echiquier, (*scoreNoirs), (*scoreBlancs));
            Case* caseRoyale = Echiquier[joueurCourant[4]->x][joueurCourant[4]->y];
            if (couleurJoueurCourant == BLANC) {
                if (caseRoyale->estAtteignableParJoueur[joueurAdverse[4]->couleur] > 0) { printf("Échec et mat : Victoire des Noirs (Rouges) !\033[K\n"); }
                else { printf("Pat !\n"); }
            } else {
                if (caseRoyale->estAtteignableParJoueur[joueurAdverse[4]->couleur] > 0) { printf("Échec et mat : Victoire des Blancs (Bleus) !\033[K\n"); } 
                else { printf("Pat !\n"); }
            }
            break;  // On quitte la partie
        }

        if (type == PVP || ((type == PVE) && (joueurCourant == Blancs))) { // Si on est en PvP ou que c'est le tour du joueur
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
                afficheEchiquier(Echiquier, (*scoreBlancs), (*scoreNoirs));
                printf("Sélectionnez une pièce à jouer à l'aide des touches directionnelles (appuyez sur 'q' pour quitter)\033[K\n");
                pthread_mutex_lock(&couleurJoueurCourant_mutex);
                printf("Joueur courant : %s\033[K\n", couleurJoueurCourant == BLANC ? "Blancs (Bleus)" : "Noirs (Rouges)");
                pthread_mutex_unlock(&couleurJoueurCourant_mutex);
                // Sauvegarder les paramètres originaux du terminal
                struct termios orig_termios;
                tcgetattr(STDIN_FILENO, &orig_termios);

                // Configurer le terminal en mode "raw"
                set_terminal_raw_mode();

                char actionJoueur;
                if (read(STDIN_FILENO, &actionJoueur, 1) != 1) { exit(EXIT_FAILURE); } // Écrit l'entrée utilisateur lue dans &actionJoueur et vérifie que cela ai fonctionné

                // Lorsque le joueur décide de quitter la partie
                if (actionJoueur == 'q') {
                    pthread_mutex_lock(&doitArreterTimer_mutex);
                    doitArreterTimer = true;
                    pthread_mutex_unlock(&doitArreterTimer_mutex);

                    pthread_mutex_lock(&couleurJoueurCourant_mutex);

                    // Pour la sauvegarde
                    pieceCourante->estSelectionnee = false;
                    if (caseCourante != NULL) { caseCourante->estSelectionnee = false; }
                    
                    // On rétablit les paramètres originaux du terminal
                    reset_terminal_mode(&orig_termios);

                    char reponse; 
                    printf("Voulez-vous sauvegarder la partie ? (o/n) : \033[K");
                    scanf("%c", &reponse);
                    
                    bool reponseValide = false;
                    while (!reponseValide) {
                        if (reponse == 'o') {
                            printf("Enregistrement de la partie...\n");
                            if (sauvegarderEchiquier(Blancs, (*scoreBlancs), Noirs, (*scoreNoirs), couleurJoueurCourant, "sauvegardes") == EXIT_FAILURE) {
                                printf("Sauvegarde impossible.\033[K\n");
                                reponse = 'n'; // On commence une nouvelle partie
                            } else { reponseValide = true; }
                        } else if (reponse == 'n') {
                            printf("Sortie de la partie\n");
                            reponseValide = true;
                        } else {
                            printf("Réponse incorrecte. Veuillez répondre par 'o' ou 'n' : \033[K");
                            scanf("%c", &reponse);
                        }
                    }

                    pthread_mutex_unlock(&couleurJoueurCourant_mutex);

                    finPartie = true;
                    break; // On quitte la partie
                }

                // Lorsque le temps imparti est écoulé
                else if (doitArreterTimer) {
                    // On rétablit les paramètres originaux du terminal
                    reset_terminal_mode(&orig_termios);

                    pthread_mutex_lock(&couleurJoueurCourant_mutex);
                    printf("Temps écoulé ! Victoire des %s\033[K\n", couleurJoueurCourant == BLANC ? "Noirs (Rouges)" : "Blancs (Bleus)");
                    pthread_mutex_unlock(&couleurJoueurCourant_mutex);
                    
                    finPartie = true;
                    break; // On quitte la partie
                }

                // Lorsque le joueur sélectionne une pièce
                else if (menu == PIECES) {
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
                                    printf("Caractère incorrect\033[K\n");
                                    break;
                            }
                        } else {
                            // Lorsque le chat du joueur marche sur son clavier
                            printf("Caractère incorrect\033[K\n");
                        }
                    }
                // Lorque le joueur sélectionne une case atteignable par la pièce validée
                } else if (menu == COUPS) {
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
                                    printf("Caractère incorrect\033[K\n");
                                    break;
                            }
                        } else {
                            // Lorsque le chat du joueur marche sur son clavier
                            printf("Caractère incorrect\033[K\n");
                        }
                    
                    // Lorque le joueur valide une case atteignable
                    } else if (actionJoueur == '\n') { // Entrée - Le coup est validé
                        // Gestion du coup
                        mouvement(Echiquier, pieceCourante, caseCourante, false, scoreCourant); // On effectue le mouvement

                        // Gestions des promotions
                        pthread_mutex_lock(&couleurJoueurCourant_mutex);
                        int rangeePromotion = (couleurJoueurCourant == BLANC) ? 7 : 0;
                        pthread_mutex_unlock(&couleurJoueurCourant_mutex);
                        if ((pieceCourante->role == PION) && (caseCourante->x == rangeePromotion)) {
                            char reponse;
                            printf("Promotion du pion ! Choisissez une pièce (R,F,T,C) : \033[K");
                            scanf("%c", &reponse);
                            
                            bool reponseValide = false;
                            while (!reponseValide) {
                                if (reponse == 'R') {
                                    printf("Promotion en Reine...\033[K\n");
                                    pieceCourante->role = REINE;
                                    pieceCourante->forme = "♛";
                                    pieceCourante->calculAtteignable = calculAtteignableReine;
                                    
                                    reponseValide = true;
                                } else if (reponse == 'F') {
                                    printf("Promotion en Fou...\033[K\n");
                                    pieceCourante->role = FOU;
                                    pieceCourante->forme = "♝";
                                    pieceCourante->calculAtteignable = calculAtteignableFou;
                                    
                                    reponseValide = true;
                                } else if (reponse == 'T') {
                                    printf("Promotion en Tour...\033[K\n");
                                    pieceCourante->role = TOUR;
                                    pieceCourante->forme = "♜";
                                    pieceCourante->calculAtteignable = calculAtteignableTour;
                                    
                                    reponseValide = true;
                                
                                } else if (reponse == 'C') {
                                    printf("Promotion en Cavalier...\033[K\n");
                                    pieceCourante->role = CAVALIER;
                                    pieceCourante->forme = "♞";
                                    pieceCourante->calculAtteignable = calculAtteignableCavalier;
                                    
                                    reponseValide = true;
                                } else {
                                    printf("Réponse incorrecte. Veuillez répondre par 'R','F','T' ou 'C' : \033[K");
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

                        // On commute la couleur
                        pthread_mutex_lock(&couleurJoueurCourant_mutex);
                        couleurJoueurCourant = (couleurJoueurCourant == BLANC) ? NOIR : BLANC;
                        pthread_mutex_unlock(&couleurJoueurCourant_mutex);

                        // On commute les score
                        scoreCourant = (scoreCourant == scoreBlancs) ? scoreNoirs : scoreBlancs;

                        // On commute le joueur
                        joueurCourant = (joueurCourant == Blancs) ? Noirs : Blancs;
                        joueurAdverse = (joueurAdverse == Noirs) ? Blancs : Noirs;
                        
                        // On vide les cases atteignables par la pièce courante pour ne plus les afficher (On ne connait pas encore la piece courante de l'adversaire)
                        actualiseCasesAtteignablesParPiece(NULL, pieceCourante);

                    }
                    
                } 
                
                // On attend la réponse du joueur
                else { continue; }

                // On rétablit les paramètres originaux
                reset_terminal_mode(&orig_termios);
            }
        } else {
            afficheEchiquier(Echiquier, (*scoreBlancs), (*scoreNoirs));
            sleep(1); // On considère que l'IA met 3 secondes minimum à réflechir.
            mouvementIA(Echiquier, joueurCourant, scoreCourant, indicePieceCourante);
                            
            // On donne la main à l'adversaire
            menu = PIECES;
            aJoue = true;

            // On commute la couleur
            pthread_mutex_lock(&couleurJoueurCourant_mutex);
            couleurJoueurCourant = (couleurJoueurCourant == BLANC) ? NOIR : BLANC;
            pthread_mutex_unlock(&couleurJoueurCourant_mutex);

            // On commute les score
            scoreCourant = (scoreCourant == scoreBlancs) ? scoreNoirs : scoreBlancs;

            // On commute le joueur
            joueurCourant = (joueurCourant == Blancs) ? Noirs : Blancs;
            joueurAdverse = (joueurAdverse == Noirs) ? Blancs : Noirs;
        }
    }
        
    videEchiquier(Echiquier); // Libère les cases
    videJoueur(Blancs); // Libère les pièces blanches
    videJoueur(Noirs);  // Libère les pièces noires 
    free(scoreNoirs);
    free(scoreBlancs);

    pthread_join(timerThread, NULL); // On attend la fin du thread du timer
    
    printf("Partie terminée !\033[K\n"); 
    exit(EXIT_SUCCESS);
}

void jeuEchec() {
    /*
    Démarre le jeu d'Echec.
    */
    
    // TODO - Proposer de jouer contre un joueur ou contre une IA

    Case* Echiquier[8][8]; // Déclaration de l'echiquier
    initialiseEchiquier(Echiquier);

    Piece *Blancs[16], *Noirs[16];  // Déclaration des joueurs
    int* scoreNoirs = malloc(sizeof(int));
    int* scoreBlancs = malloc(sizeof(int));

    char reponse; 
    printf("Voulez-vous charger une sauvegarde ? (o/n) : \033[K");
    scanf("%c", &reponse);

    pthread_mutex_lock(&couleurJoueurCourant_mutex);

    bool reponseValide = false;
    while (!reponseValide) {
        if (reponse == 'o') {
            printf("Chargement de la sauvegarde...\033[K\n");
            // Initialisations par chargement
            if (chargerEchiquier(Echiquier, Blancs, scoreBlancs, Noirs, scoreNoirs, "sauvegardes") == EXIT_FAILURE) {
                printf("Fichiers de sauvegarde corrompus. Création d'une nouvelle partie.\033[K\n");
                reponse = 'n'; // On commence une nouvelle partie
            } else { reponseValide = true; }
        } else if (reponse == 'n') {
            printf("Nouvelle partie...\033[K\n");
            // Initialisations par défaut
            initialiseJoueur(Echiquier, Blancs, BLANC);
            initialiseJoueur(Echiquier, Noirs, NOIR);
            *scoreBlancs = 0;
            *scoreNoirs = 0;
            couleurJoueurCourant = BLANC; // Les blancs commencent
            reponseValide = true;
        } else {
            printf("Réponse incorrecte. Veuillez répondre par 'o' ou 'n' : \033[K");
            scanf("%c", &reponse);
        }
    }

    pthread_mutex_unlock(&couleurJoueurCourant_mutex);

    actualiseCasesAtteignablesParJoueur(Echiquier, Blancs);
    actualiseCasesAtteignablesParJoueur(Echiquier, Noirs);

    printf("Voulez-vous jouer contre l'IA ? (o/n) : \033[K");
    scanf("%c", &reponse);

    reponseValide = false;
    while (!reponseValide) {
        if (reponse == 'o') {
            printf("Lancement d'une partie contre l'IA. Appuyez sur Entrée pour commencer...\033[K\n");
            getchar();
            while (getchar() != '\n');
            partieEchec(Echiquier, Blancs, scoreBlancs, Noirs, scoreNoirs, PVE);
        } else if (reponse == 'n') {
            printf("Lancement d'une partie joueur contre joueur. Appuyez sur Entrée pour commencer...\033[K\n");
            getchar();
            while (getchar() != '\n');
            partieEchec(Echiquier, Blancs, scoreBlancs, Noirs, scoreNoirs, PVP);
        } else if (reponse == 'b') {
            printf("Lancement d'une partie IA contre IA. Appuyez sur Entrée pour commencer...\033[K\n");
            getchar();
            while (getchar() != '\n');
            partieEchec(Echiquier, Blancs, scoreBlancs, Noirs, scoreNoirs, EVE);
        } else {
            printf("Réponse incorrecte. Veuillez répondre par 'o' ou 'n' : \033[K");
            scanf("%c", &reponse);
        }
    }


   


}