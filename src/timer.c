#include <pthread.h> // pthread_create()
#include <time.h> // sleep()
#include <stdbool.h> // bool
#include <stdio.h> // printf()

#include ".././include/lecture.h" // deplacerCurseur()
#include ".././include/echiquier.h" // couleurJoueurCourant, doitArreterTimer

#include ".././include/timer.h"

volatile int tempsImpartiBlancs = TEMPS_IMPARTI; 
volatile int tempsImpartiNoirs = TEMPS_IMPARTI; 

void* threadTimer() {
    /*
    Gère le timer de la partie.
    */

    while (!doitArreterTimer) {
        sleep(1); // On attend une seconde

        couleurJoueurCourant == 1 ? tempsImpartiBlancs-- : tempsImpartiNoirs--; // On décrémente le temps imparti du joueur courant

        if (tempsImpartiBlancs == 0 || tempsImpartiNoirs == 0) {
            doitArreterTimer = true;
            break;
        } else {
            // Save the cursor position
            printf("\033[s");

            deplacerCurseur(1, 1); // On se déplace en haut à gauche du terminal
            printf("\033[K");
            printf("Temps restant : %d:%d", couleurJoueurCourant == 1 ? tempsImpartiBlancs : tempsImpartiNoirs, couleurJoueurCourant == 1 ? tempsImpartiNoirs : tempsImpartiBlancs);
        
            printf("\033[u");
            fflush(stdout);  // Ensure the output is printed immediately
        }

    }

    return NULL;
}