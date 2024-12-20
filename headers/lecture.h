#ifndef LECTURE_H
#define LECTURE_H

/* Fonctions pour désactiver la nécessitée d'appuyer sur "Entrée" après avoir écrit dans le terminal */

// Configuration du terminal
void set_terminal_raw_mode();
void reset_terminal_mode(struct termios *orig);

#endif // LECTURE_H