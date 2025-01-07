#ifndef TIMER_H
#define TIMER_H

#define TEMPS_IMPARTI 600 // Temps imparti pour chaque joueur, en secondes (10 minutes)

#include <stdbool.h> // bool
#include <pthread.h> // pthread_mutex_t

extern volatile int couleurJoueurCourant;
extern pthread_mutex_t couleurJoueurCourant_mutex;

extern volatile bool doitArreterTimer;
extern pthread_mutex_t doitArreterTimer_mutex;


void* threadTimer();

#endif // TIMER_H