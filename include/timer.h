#ifndef TIMER_H
#define TIMER_H

#define TEMPS_IMPARTI 15 // Temps imparti pour chaque joueur, en secondes (5 minutes)

#include <stdbool.h> // bool

extern volatile int couleurJoueurCourant;
extern volatile bool doitArreterTimer;

void* threadTimer();

#endif // TIMER_H